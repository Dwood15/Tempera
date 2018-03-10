#include <string.h>
#include <exception>
#include "command.h"
#include "../../version.h"
#include "../messaging/messaging.h"
#include "../settings.h"

const char *ChimeraCommand::name() const noexcept {
    return this->i_name.data();
}

const char *ChimeraCommand::category() const noexcept {
    return this->i_category.data();
}

const char *ChimeraCommand::help() const noexcept {
    return this->i_help.data();
}

bool ChimeraCommand::supported() const noexcept {
    return this->i_supported;
}

bool ChimeraCommand::saves() const noexcept {
    return this->i_saves;
}

ChimeraCommandError ChimeraCommand::execute(int argc, const char **argv) const noexcept {
    if(argc < this->i_min_args)
        return CHIMERA_COMMAND_ERROR_NOT_ENOUGH_ARGUMENTS;
    else if(argc > this->i_max_args)
        return CHIMERA_COMMAND_ERROR_TOO_MANY_ARGUMENTS;
    else if(!this->i_supported)
        return CHIMERA_COMMAND_ERROR_UNSUPPORTED;
    else {
        auto r = this->i_function(argc, argv);
        if(r == CHIMERA_COMMAND_ERROR_SUCCESS && this->i_saves && argc > 0)
            commit_command(this->name(), argc, argv);
        return r;
    }
}

size_t ChimeraCommand::min_args() const noexcept {
    return this->i_min_args;
}

size_t ChimeraCommand::max_args() const noexcept {
    return this->i_max_args;
}

ChimeraCommand::ChimeraCommand(const char *name, ChimeraCommandFunctionPointer function, const char *category, const char *help, size_t min_args, size_t max_args, bool supported, bool saves) :
    i_name(name),
    i_supported(supported),
    i_saves(saves),
    i_function(function),
    i_min_args(min_args) {
    if(category == nullptr) this->i_category = "";
    else this->i_category = category;
    if(help == nullptr) this->i_help = "";
    else this->i_help = help;
    if(max_args == 0)
        this->i_max_args = min_args;
    else if(min_args > max_args)
        throw std::exception();
    else
        this->i_max_args = max_args;
}

std::vector<std::string> split_arguments(const char *input, bool lowercase_first_argument) {
    std::vector<const char *> args;
    std::string input_copy = input;
    char *data = &input_copy[0];
    args.push_back(data);
    size_t length = input_copy.size();
    bool in_quotes = false;
    for(size_t i=0;i<length;i++) {
        char *c = &data[i];
        if(*c == '"' && !(in_quotes = !in_quotes)) {
            *c = ' ';
        }

        if(!in_quotes && (*c == 0 || *c == ' ')) {
            while(*c == ' ') {
                *c = 0;
                i++;
                c++;
            }
            if(i < length) args.push_back(&data[i]);
            i--;
        }
    }

    std::vector<std::string> args_cleaned;
    for(size_t i=0;i<args.size();i++) {
        std::string arg;
        size_t sl = strlen(args[i]);
        for(size_t c=0;c<sl;c++) {
            if(args[i][c] == '"') continue;
            if(lowercase_first_argument && args_cleaned.size() == 0) {
                arg += tolower(args[i][c]);
            }
            else {
                arg += args[i][c];
            }
        }
        args_cleaned.push_back(arg);
    }

    return args_cleaned;
}

extern std::vector<ChimeraCommand> *commands;
ChimeraCommandError execute_chimera_command(const char *command_input, bool silently, bool save) {
    if(commands != nullptr && command_input != nullptr) {
        auto args_cleaned = split_arguments(command_input, true);
        if(args_cleaned.size() == 0) return CHIMERA_COMMAND_ERROR_COMMAND_NOT_FOUND;
        std::vector<const char *> args;
        for(size_t i=0;i<args_cleaned.size();i++) {
            args.push_back(args_cleaned[i].data());
        }
        for(size_t i=0;i<commands->size();i++) {
            ChimeraCommand &command = (*commands)[i];
            if(args_cleaned[0] == command.name()) {
                extern const char *current_command;
                const char *older_command = current_command;
                if(strcmp(command.name(),"chimera") != 0) current_command = command.name();
                extern bool silence_all_messages;
                bool already_silent = silence_all_messages;
                if(silently && !already_silent) {
                    silence_all_messages = true;
                }
                auto rval = CHIMERA_COMMAND_ERROR_COMMAND_NOT_FOUND;

                auto read_only = settings_read_only();
                if(!save) settings_read_only(1);
                if(args.size() == 1)
                    rval = command.execute();
                else
                    rval = command.execute(args.size() - 1, &args[1]);
                if(!save) settings_read_only(read_only);
                if(silently && !already_silent) {
                    silence_all_messages = false;
                }
                current_command = older_command;
                return rval;
            }
        }
    }
    return CHIMERA_COMMAND_ERROR_COMMAND_NOT_FOUND;
}

ChimeraCommand &find_chimera_command(const char *command_input) {
    if(commands != nullptr && command_input != nullptr) {
        auto command_name = split_arguments(command_input, true)[0];
        for(size_t i=0;i<commands->size();i++) {
            ChimeraCommand &command = (*commands)[i];
            if(command_name == command.name()) return command;
        }
    }
    throw std::exception();
}

bool bool_value(const char *v) noexcept {
    return (!strcmp(v,"1")) || (!strcmp(v,"true"));
}

ChimeraCommandError chimera_command(size_t argc, const char **argv) noexcept {
    std::vector<std::string> list;
    if(argc == 0) {
        std::vector<std::string> list_a;
        const auto version_color = ColorARGB(1.0, 1.0, 0.478, 0.552);
        console_out("Chimera build " CHIMERA_BUILD_STRING " by " CHIMERA_AUTHOR, version_color);
        console_out("Lua API version (clua_version): " STR(CHIMERA_LUA_INTERPRETER), version_color);
        for(size_t i=0;i<(*commands).size();i++) {
            auto &command = (*commands)[i];
            if(!command.supported()) continue;
            const char *string = command.category();
            if(strcmp(string,"") == 0) continue;
            bool added = false;
            for(size_t k=0;k<list_a.size();k++) {
                if(list_a[k] == string) {
                    added = true;
                    break;
                }
            }
            if(!added) {
                list_a.emplace_back(string);
            }
        }
        if(list_a.size() == 0) {
            console_out("There are no available categories.");
        }
        else {
            for(size_t i=0;i<list_a.size();i++) {
                const char *string = list_a[i].data();
                bool added = false;
                for(size_t k=0;k<list.size();k++) {
                    if(list[k].compare(string) > 0) {
                        list.insert(list.begin() + k, string);
                        added = true;
                        break;
                    }
                }
                if(!added) {
                    list.emplace_back(string);
                }
            }
            console_out("Categories:");
            for(size_t i=0;i<list.size();i++) {
                char z[256] = {};
                sprintf(z,"  - %s", list[i].data());
                console_out(z);
            }
            console_out("Use \"chimera <category>\" to list the commands in each category.");
        }
    }
    else if(argc == 1) {
        std::string category = argv[0];
        for(size_t i=0;i<category.size();i++) {
            category[i] = tolower(category[i]);
        }
        if(category == "issues") {
            return chimera_command(0, nullptr);
        }
        else if(category == "<category>") {
            console_out_error("chimera: You need to replace <category> with the name of the category");
            return CHIMERA_COMMAND_ERROR_FAILURE;
        }
        else if(category == "the name of the category") {
            console_out("chimera: BLOOD GULCH SUCKS");
            return CHIMERA_COMMAND_ERROR_SUCCESS;
        }
        for(size_t i=0;i<(*commands).size();i++) {
            auto &command = (*commands)[i];
            if(!command.supported()) continue;
            if(category != command.category()) continue;
            const char *string = command.name();
            bool added = false;
            for(size_t k=0;k<list.size();k++) {
                if(list[k].compare(string) > 0) {
                    list.insert(list.begin() + k, string);
                    added = true;
                    break;
                }
            }
            if(!added) {
                list.emplace_back(string);
            }
        }
        if(strcmp(argv[0],"") == 0) {
            list.clear();
        }
        char x[256] = {};
        if(list.size() != 0) {
            sprintf(x, "chimera: Commands in \"%s\":", category.data());
            console_out(x);
            for(size_t i=0;i<list.size();i++) {
                char z[256] = {};
                sprintf(z,"  - %s", list[i].data());
                console_out(z);
            }
            console_out("Use \"chimera <command>\" to get help for a command.");
        }
        else {
            if(category == "<command>") {
                console_out_error("chimera: You need to replace <command> with the name of the command");
                return CHIMERA_COMMAND_ERROR_FAILURE;
            }
            else if(category == "the name of the command") {
                console_out("chimera: GENERATION II WAS BETTER THAN GENERATION I");
                return CHIMERA_COMMAND_ERROR_SUCCESS;
            }
            for(int z=0;z<2;z++) {
                char m[256] = {};
                if(z == 0) {
                    sprintf(m, "%s", category.data());
                }
                else {
                    sprintf(m, "chimera_%s", category.data());
                }
                try {
                    auto &command_found = find_chimera_command(m);
                    std::string help = command_found.help();
                    if(help == "") {
                        sprintf(x, "chimera: No help available for \"%s\"...", command_found.name());
                        console_out_error(x);
                    }
                    else {
                        std::vector<const char *> lines;
                        lines.push_back(help.data());
                        for(size_t i=0;i<help.size();i++) {
                            if(help[i] == '\n') {
                                help[i] = 0;
                                lines.push_back(&help[i + 1]);
                            }
                        }
                        memset(x,0,sizeof(x));
                        sprintf(x, "chimera: Help for \"%s\":", command_found.name());
                        console_out(x);
                        ColorARGB color;
                        color.red = 0.7;
                        for(size_t i=0;i<lines.size();i++) {
                            memset(x,0,sizeof(x));
                            sprintf(x,"> %s", lines[i]);
                            console_out(x, color);
                        }
                        if(!command_found.saves()) {
                            console_out("This command does not automatically save.", color);
                        }
                    }
                    break;
                }
                catch(std::exception &) {
                    if(z == 1) {
                        sprintf(x, "chimera: Could not find command or category \"%s\".", category.data());
                        console_out_error(x);
                        console_out_error("chimera: Check your spelling and try again.");
                        return CHIMERA_COMMAND_ERROR_FAILURE;
                    }
                }
            }
        }
    }
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
