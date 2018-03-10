#include "lua_callback.h"

#include <memory>

static LARGE_INTEGER last_time;

#include "../messaging/messaging.h"
#include "../hooks/camera.h"
#include "../hooks/frame.h"
#include "../hooks/map_load.h"
#include "../hooks/rcon_message.h"
#include "../hooks/tick.h"
#include "../interpolation/camera.h"

extern std::vector<std::unique_ptr<LuaScript>> scripts;

#define call_all_priorities(function) function(EVENT_PRIORITY_BEFORE); function(EVENT_PRIORITY_DEFAULT); function(EVENT_PRIORITY_AFTER); function(EVENT_PRIORITY_FINAL)
//#define pcall(state, args, result_count) if(lua_pcall(state, args, result_count, 0) != LUA_OK) { print_error(state); }

int pcall(lua_State *state, int args, int result_count) noexcept {
    auto x = lua_pcall(state, args, result_count, 0);
    if(x != LUA_OK) print_error(state);
    return x;
}

#define basic_callback(callback) [](EventPriority priority) noexcept {\
    for(size_t i=0;i<scripts.size();i++) {\
        auto &script = *scripts[i].get();\
        auto &script_callback = script.callback;\
        if(script_callback.callback_function != "" && script_callback.priority == priority) {\
            auto *&state = script.state;\
            lua_getglobal(state, script_callback.callback_function.data());\
            pcall(state, 0, 0);\
        }\
    }\
};

extern void load_map_script() noexcept;

static void check_timers() noexcept {
    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    auto r = counter_time_elapsed(last_time, now_time);
    last_time = now_time;
    for(size_t i=0;i<scripts.size();i++) {
        auto &script = *scripts[i];
        auto timers = script.timers;
        for(size_t t=0;t<timers.size();t++) {
            auto &timer = timers[t];
            timer.time_passed += r * 1000;
            bool deleted = false;
            while(timer.time_passed >= timer.interval_ms) {
                deleted = true;
                for(size_t to=0;to<script.timers.size();to++) {
                    auto &timer_to = script.timers[to];
                    if(timer.timer_id == timer_to.timer_id) {
                        deleted = false;
                        break;
                    }
                }
                if(deleted) break;
                lua_getglobal(script.state, timer.function.data());
                for(size_t arg=0;arg<timer.arguments.size();arg++) {
                    timer.arguments[arg].push_argument(script);
                }
                if(pcall(script.state, timer.arguments.size(), 1) == LUA_OK) {
                    if(lua_isboolean(script.state, -1) && !lua_toboolean(script.state, -1)) {
                        deleted = true;
                        lua_pop(script.state, 1);
                        break;
                    }
                    lua_pop(script.state, 1);
                }
                timer.time_passed -= timer.interval_ms;
            }
            for(size_t to=0;to<script.timers.size();to++) {
                auto &timer_to = script.timers[to];
                if(timer.timer_id == timer_to.timer_id) {
                    if(deleted) {
                        script.timers.erase(script.timers.begin() + to);
                    }
                    else {
                        timer_to.time_passed = timer.time_passed;
                    }
                    break;
                }
            }
        }
    }
}

static void map_load_callback() noexcept {
    for(size_t i=0;i<scripts.size();i++) {
        if(!scripts[i].get()->global) {
            scripts.erase(scripts.begin() + i);
            break;
        }
    }
    load_map_script();
    for(size_t i=0;i<scripts.size();i++) {
        refresh_variables(scripts[i].get()->state);
    }
    auto x = basic_callback(c_map_load);
    call_all_priorities(x);
}

static void pretick_callback() noexcept {
    for(size_t i=0;i<scripts.size();i++) {
        refresh_client_index(scripts[i].get()->state);
    }
    auto x = basic_callback(c_pretick);
    call_all_priorities(x);
}

static void tick_callback() noexcept {
    check_timers();
    auto x = basic_callback(c_tick);
    call_all_priorities(x);
}

static void preframe_callback() noexcept {
    auto x = basic_callback(c_preframe);
    call_all_priorities(x);
}

static void frame_callback() noexcept {
    check_timers();
    auto x = basic_callback(c_frame);
    call_all_priorities(x);
}

static void camera_callback() noexcept {
    auto &data = camera_data();

    auto x = [&data](EventPriority priority) {
        for(size_t i=0;i<scripts.size();i++) {
            auto &script = *scripts[i].get();
            auto &script_callback = script.c_precamera;
            if(script_callback.callback_function != "" && script_callback.priority == priority) {
                auto *&state = script.state;
                lua_getglobal(state, script_callback.callback_function.data());
                lua_pushnumber(state, data.position.x);
                lua_pushnumber(state, data.position.y);
                lua_pushnumber(state, data.position.z);
                lua_pushnumber(state, data.fov);
                lua_pushnumber(state, data.orientation[0].x);
                lua_pushnumber(state, data.orientation[0].y);
                lua_pushnumber(state, data.orientation[0].z);
                lua_pushnumber(state, data.orientation[1].x);
                lua_pushnumber(state, data.orientation[1].y);
                lua_pushnumber(state, data.orientation[1].z);
                if(pcall(state, 10, 10) == LUA_OK) {
                    if(priority != EVENT_PRIORITY_FINAL) {
                        #define set_if_possible(val, i) if(lua_isnumber(state, i)) val = lua_tonumber(state, i)
                        set_if_possible(data.position.x, -10);
                        set_if_possible(data.position.y, -9);
                        set_if_possible(data.position.z, -8);
                        set_if_possible(data.fov, -7);
                        set_if_possible(data.orientation[0].x, -6);
                        set_if_possible(data.orientation[0].y, -5);
                        set_if_possible(data.orientation[0].z, -4);
                        set_if_possible(data.orientation[1].x, -3);
                        set_if_possible(data.orientation[1].y, -2);
                        set_if_possible(data.orientation[1].z, -1);
                    }
                    lua_pop(state, 10);
                }
            }
        }
    };
    call_all_priorities(x);
}

#define allow_string_callback(callback) [](EventPriority priority, const char *string, bool &allow) noexcept {\
    for(size_t i=0;i<scripts.size() && allow;i++) {\
        auto &script = *scripts[i].get();\
        auto &script_callback = script.callback;\
        if(script_callback.callback_function != "" && script_callback.priority == priority) {\
            auto *&state = script.state;\
            lua_getglobal(state, script_callback.callback_function.data());\
            lua_pushstring(state, string);\
            if(pcall(state, 1, 1) == LUA_OK) {\
                if(!lua_isnil(state,-1) && priority != EVENT_PRIORITY_FINAL) {\
                    allow = lua_toboolean(state,-1);\
                    if(script.version < 2.02) allow = !allow; /* BC */ \
                }\
                lua_pop(state,1);\
            }\
        }\
    }\
};
#define call_all_priorities_allow_str(function,str,allow) function(EVENT_PRIORITY_BEFORE,str,allow); function(EVENT_PRIORITY_DEFAULT,str,allow); function(EVENT_PRIORITY_AFTER,str,allow); function(EVENT_PRIORITY_FINAL,str,allow)

bool on_command_lua(const char *command) noexcept {
    bool allow = true;
    auto x = allow_string_callback(c_command);
    call_all_priorities_allow_str(x, command, allow);
    return allow;
}

bool rcon_message_callback(const char *message) noexcept {
    bool allow = true;
    auto x = allow_string_callback(c_rcon_message);
    call_all_priorities_allow_str(x, message, allow);
    return allow;
}

struct UnderscoreSpaceThing {
    std::string i_text;

    bool operator ==(const char *&other) const noexcept {
        return this->i_text == other;
    }

    UnderscoreSpaceThing(const char *string) noexcept : i_text(string) {
        for(size_t i=0;i<i_text.size();i++) {
            if(i_text[i] == '_') i_text[i] = ' ';
        }
    }
};

int lua_set_callback(lua_State *state) noexcept {
    int args = lua_gettop(state);

    if(args == 1 || args == 2) {
        const char *callback_name = luaL_checkstring(state,1);
        const char *function_name = "";
        if(args == 2) {
            function_name = luaL_checkstring(state,2);
            if(function_name == nullptr) function_name = "";
        }
        EventPriority priority = EVENT_PRIORITY_DEFAULT;
        if(args == 3) {
            auto callback_priority = std::string(luaL_checkstring(state,3));
            if(callback_priority == "before")
                priority = EVENT_PRIORITY_BEFORE;
            else if(callback_priority == "default")
                priority = EVENT_PRIORITY_DEFAULT;
            else if(callback_priority == "after")
                priority = EVENT_PRIORITY_AFTER;
            else if(callback_priority == "final")
                priority = EVENT_PRIORITY_FINAL;
        }

        #define cpref_(cb) c_ ## cb

        #define if_callback_then_set(cb) if(UnderscoreSpaceThing(#cb) == callback_name) {\
            auto &callback = script_from_state(state).cpref_(cb);\
            callback.callback_function = function_name;\
            callback.priority = priority;\
        }

        if_callback_then_set(command)

        else if_callback_then_set(frame)
        else if_callback_then_set(preframe)

        else if_callback_then_set(map_load)
        else if_callback_then_set(map_preload)

        else if_callback_then_set(precamera)

        else if_callback_then_set(rcon_message)

        else if_callback_then_set(spawn)
        else if_callback_then_set(prespawn)

        else if_callback_then_set(tick)
        else if_callback_then_set(pretick)

        else if_callback_then_set(unload)
        else {
            char z[256] = {};
            sprintf(z,"invalid callback %s given in set_callback",callback_name);
            return luaL_error(state,z);
        }
    }
    else luaL_error(state,"wrong number of arguments in set_callback");
    return 0;
}

void setup_callbacks() noexcept {
    add_map_load_event(map_load_callback, EVENT_PRIORITY_BEFORE);
    add_pretick_event(pretick_callback, EVENT_PRIORITY_BEFORE);
    add_tick_event(tick_callback, EVENT_PRIORITY_BEFORE);
    add_frame_event(frame_callback, EVENT_PRIORITY_BEFORE);
    add_preframe_event(preframe_callback, EVENT_PRIORITY_BEFORE);
    add_rcon_message_event(rcon_message_callback, EVENT_PRIORITY_BEFORE);
    add_precamera_event(camera_callback, EVENT_PRIORITY_AFTER);
    QueryPerformanceCounter(&last_time);
}
