#include "lua_game.h"
#include "lua_callback.h"

#include <string.h>

#include "../command/console.h"
#include "../halo_data/global.h"
#include "../halo_data/script.h"
#include "../halo_data/spawn_object.h"
#include "../halo_data/table.h"
#include "../halo_data/tag_data.h"
#include "../hooks/tick.h"
#include "../messaging/messaging.h"

static int lua_console_is_open(lua_State *state) noexcept {
    lua_pushboolean(state, console_is_out());
    return 1;
}

static int lua_console_out(lua_State *state) noexcept {
    int args = lua_gettop(state);
    if(args == 1 || args == 4 || args == 5) {
        const char *string;
        if(lua_isboolean(state,1))
            string = lua_toboolean(state,1) ? "true" : "false";
        else if(lua_isnil(state,1))
            string = "(nil)";
        else
            string = luaL_checkstring(state,1);
        ColorARGB argb;
        if(args >= 4) {
            int offset = 0;
            if(args >= 5) {
                argb.alpha = luaL_checknumber(state,2);
                offset = 1;
            }
            argb.red = luaL_checknumber(state,offset+2);
            argb.green = luaL_checknumber(state,offset+3);
            argb.blue = luaL_checknumber(state,offset+4);
        }
        console_out(string, argb);
        return 0;
    }
    else return luaL_error(state,"wrong number of arguments in console_out");
}

static int lua_delete_object(lua_State *state) noexcept {
    int args = lua_gettop(state);
    if(args == 1) {
        if(!delete_object(luaL_checknumber(state,1)))
            return luaL_error(state,"invalid object id");
        return 0;
    }
    else return luaL_error(state,"wrong number of arguments in delete_object");
}

static int lua_execute_script(lua_State *state) noexcept {
    int args = lua_gettop(state);
    if(args == 1 || args == 4 || args == 5) {
        const char *string = luaL_checkstring(state,1);
        execute_script(string);
        return 0;
    }
    else return luaL_error(state,"wrong number of arguments in execute_script");
}

static int lua_get_dynamic_player(lua_State *state) noexcept {
    int args = lua_gettop(state);
    if(args <= 1) {
        char *dynamic_player_data;
        if(args == 0) dynamic_player_data = HaloObject(HaloPlayer().object_id()).object_data();
        else dynamic_player_data = HaloObject(HaloPlayer(luaL_checkinteger(state, 1)).object_id()).object_data();
        if(dynamic_player_data) {
            lua_pushinteger(state, reinterpret_cast<uint32_t>(dynamic_player_data));
        }
        else {
            lua_pushnil(state);
        }
        return 1;
    }
    else {
        return luaL_error(state,"wrong number of arguments in get_dynamic_player");
    }
}

static int lua_get_global(lua_State *state) noexcept {
    int args = lua_gettop(state);
    if(args == 1) {
        const char *global_name = luaL_checkstring(state,1);
        ScriptingGlobal s = read_global(global_name);
        switch(s.type) {
            case SCRIPTING_GLOBAL_NOT_FOUND:
            case SCRIPTING_GLOBAL_UNIMPLEMENTED:
                return luaL_error(state,"global not found or not implemented");
            case SCRIPTING_GLOBAL_BOOLEAN:
                lua_pushboolean(state,s.value.boolean);
                return 1;
            case SCRIPTING_GLOBAL_REAL:
                lua_pushnumber(state,s.value.real);
                return 1;
            case SCRIPTING_GLOBAL_SHORT:
                lua_pushinteger(state,s.value.short_int);
                return 1;
            case SCRIPTING_GLOBAL_LONG:
                lua_pushinteger(state,s.value.long_int);
                return 1;
        }
        return 0;
    }
    else {
        return luaL_error(state,"wrong number of arguments in set_global");
    }
}

static int lua_get_object(lua_State *state) noexcept {
    int args = lua_gettop(state);
    if(args == 1) {
        char *object_data = HaloObject(luaL_checkinteger(state, 1)).object_data();
        if(object_data) {
            lua_pushinteger(state, reinterpret_cast<uint32_t>(object_data));
        }
        else {
            lua_pushnil(state);
        }
        return 1;
    }
    else {
        return luaL_error(state,"wrong number of arguments in get_object");
    }
}

static int lua_get_player(lua_State *state) noexcept {
    int args = lua_gettop(state);
    if(args <= 1) {
        char *player_data;
        if(args == 0) player_data = HaloPlayer().player_data();
        else player_data = HaloPlayer(luaL_checkinteger(state, 1)).player_data();
        if(player_data) {
            lua_pushinteger(state, reinterpret_cast<uint32_t>(player_data));
        }
        else {
            lua_pushnil(state);
        }
        return 1;
    }
    else {
        return luaL_error(state,"wrong number of arguments in get_player");
    }
}

static int lua_get_tag(lua_State *state) noexcept {
    int args = lua_gettop(state);
    if(args == 1) {
        HaloTagID tag_id = static_cast<uint32_t>(luaL_checkinteger(state, 1));
        if(tag_id.is_valid())
            lua_pushinteger(state, reinterpret_cast<uint32_t>(&HaloTag::from_id(tag_id)));
        else
            lua_pushnil(state);
        return 1;
    }
    else if(args == 2) {
        try {
            lua_pushinteger(state, reinterpret_cast<uint32_t>(&HaloTag::lookup(luaL_checkstring(state, 1), luaL_checkstring(state, 2))));
        }
        catch(...) {
            lua_pushnil(state);
        }
        return 1;
    }
    else {
        return luaL_error(state,"wrong number of arguments in get_tag");
    }
}

static int lua_hud_message(lua_State *state) noexcept {
    int args = lua_gettop(state);
    if(args == 1) {
        const char *string;
        if(lua_isboolean(state,1))
            string = lua_toboolean(state,1) ? "true" : "false";
        else if(lua_isnil(state,1))
            string = "(nil)";
        else
            string = luaL_checkstring(state,1);
        hud_message(string);
        return 0;
    }
    return luaL_error(state,"wrong number of arguments in hud_message");
}

static int lua_set_global(lua_State *state) noexcept {
    int args = lua_gettop(state);
    if(args == 2) {
        const char *global_name = luaL_checkstring(state,1);
        ScriptingGlobal value = read_global(global_name);
        switch(value.type) {
            case SCRIPTING_GLOBAL_NOT_FOUND:
            case SCRIPTING_GLOBAL_UNIMPLEMENTED:
                return luaL_error(state,"global not found or not implemented");
            case SCRIPTING_GLOBAL_BOOLEAN:
                if(lua_isboolean(state,2)) {
                    value.value.boolean = lua_toboolean(state,2);
                }
                else if(lua_isnumber(state,2)) {
                    value.value.boolean = lua_tointeger(state,2) != 0;
                }
                else {
                    return luaL_error(state,"expected a number or boolean in argument 2");
                }
                break;
            case SCRIPTING_GLOBAL_REAL:
                if(lua_isnumber(state,2)) {
                    value.value.real = lua_tonumber(state,2);
                }
                else {
                    return luaL_error(state,"expected a number in argument 2");
                }
                break;
            case SCRIPTING_GLOBAL_SHORT:
                if(lua_isnumber(state,2)) {
                    value.value.short_int = lua_tointeger(state,2);
                }
                else {
                    return luaL_error(state,"expected a number in argument 2");
                }
                break;
            case SCRIPTING_GLOBAL_LONG:
                if(lua_isnumber(state,2)) {
                    value.value.long_int = lua_tointeger(state,2);
                }
                else {
                    return luaL_error(state,"expected a number in argument 2");
                }
                break;
        }
        set_global(global_name, value.value);
        return 0;
    }
    else {
        return luaL_error(state,"wrong number of arguments in set_global");
    }
}

static int lua_set_timer(lua_State *state) noexcept {
    int args = lua_gettop(state);
    if(args >= 2) {
        auto &script = script_from_state(state);
        auto interval = luaL_checknumber(state, 1);
        if(interval < 0.1) {
            return luaL_error(state,"interval must be at least 0.1 millseconds");
        }
        auto *function = luaL_checkstring(state, 2);
        LuaScriptTimer timer;
        timer.timer_id = script.next_timer_id++;
        timer.interval_ms = interval;
        timer.function = function;
        for(size_t i=3;i<=args;i++) {
            timer.arguments.push_back(LuaAmbiguousTypeArgument::check_argument(script, i, true));
        }
        lua_pushinteger(state, timer.timer_id);
        script.timers.emplace_back(timer);
        return 1;
    }
    else {
        return luaL_error(state,"wrong number of arguments in set_timer");
    }
}

static int lua_stop_timer(lua_State *state) noexcept {
    int args = lua_gettop(state);
    if(args == 1) {
        auto &script = script_from_state(state);
        auto id = luaL_checkinteger(state, 1);
        for(size_t i=0;i<script.timers.size();i++) {
            if(script.timers[i].timer_id == id) {
                script.timers.erase(script.timers.begin() + i);
                return luaL_error(state,"timer with that ID does not exist");
            }
        }
        return 0;
    }
    else {
        return luaL_error(state,"wrong number of arguments in stop_timer");
    }
}

static int lua_spawn_object(lua_State *state) noexcept {
    int args = lua_gettop(state);
    if(args == 5 || args == 4) {
        int a;
        uint32_t tag_id = 0xFFFFFFFF;
        if(lua_isnumber(state, 1)) {
            tag_id = luaL_checknumber(state, tag_id);
            a = 1;
        }
        else {
            try {
                tag_id = HaloTag::lookup(luaL_checkstring(state, 1), luaL_checkstring(state, 2)).id;
            }
            catch(...) {
                return luaL_error(state,"could not find tag");
            }
            a = 2;
        }
        if(static_cast<uint16_t>(tag_id) >= *reinterpret_cast<uint32_t *>(0x4044000C)) {
            return luaL_error(state,"invalid object ID specified in spawn_object");
        }
        float x = luaL_checknumber(state, a + 1);
        float y = luaL_checknumber(state, a + 2);
        float z = luaL_checknumber(state, a + 3);
        auto object_id = spawn_object(tag_id, x, y, z);
        lua_pushinteger(state, object_id);
        return 1;
    }
    else {
        return luaL_error(state,"wrong number of arguments in spawn_object");
    }
}

static int lua_tick_rate(lua_State *state) noexcept {
    int args = lua_gettop(state);
    if(args == 1) {
        auto value = luaL_checknumber(state, 1);
        if(value < 0.01) return luaL_error(state, "tick rate must be at least 0.01");
        set_tick_rate(luaL_checknumber(state, 1));
    }
    lua_pushnumber(state, tick_rate());
    return 1;
}

static int lua_ticks(lua_State *state) noexcept {
    auto progress = tick_progress();
    if(progress > 0.99999) progress = 0.99999;
    lua_pushnumber(state,static_cast<double>(tick_count()) + progress);
    return 1;
}

void set_game_functions(lua_State *state) noexcept {
    lua_register(state, "console_is_open", lua_console_is_open);
    lua_register(state, "console_out", lua_console_out);
    lua_register(state, "delete_object", lua_delete_object);
    lua_register(state, "execute_script", lua_execute_script);
    lua_register(state, "get_dynamic_player", lua_get_dynamic_player);
    lua_register(state, "get_global", lua_get_global);
    lua_register(state, "get_object", lua_get_object);
    lua_register(state, "get_player", lua_get_player);
    lua_register(state, "get_tag", lua_get_tag);
    lua_register(state, "hud_message", lua_hud_message);
    lua_register(state, "set_callback", lua_set_callback);
    lua_register(state, "set_global", lua_set_global);
    lua_register(state, "set_timer", lua_set_timer);
    lua_register(state, "stop_timer", lua_stop_timer);
    lua_register(state, "spawn_object", lua_spawn_object);
    lua_register(state, "tick_rate", lua_tick_rate);
    lua_register(state, "ticks", lua_ticks);
}
