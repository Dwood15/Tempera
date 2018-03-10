#include "settings.h"
#include "path.h"
#include "../version.h"

#include <iostream>
#include <fstream>
#include <string>

#include "messaging/messaging.h"

bool settings_read_only(int new_value) noexcept {
    static bool value = 0;
    if(new_value > -1) value = new_value == 1;
    return value;
}

bool settings_do_not_save(int new_value) noexcept {
    static bool value = 0;
    if(new_value > -1) value = new_value == 1;
    return value;
}

const char *startup_path() {
    static std::string startup_path_s;
    static bool set = false;
    if(!set) {
        char p[MAX_PATH] = {};
        GetTempPath(MAX_PATH, p);
        startup_path_s = std::string(p) + "\\chimerastartup.bin";
    }
    return startup_path_s.data();
}

static std::vector<std::vector<std::string>> save_data;

void commit_command(const char *command, size_t argc, const char **argv) noexcept {
    if(settings_read_only()) return;
    std::vector<std::string> new_data;
    new_data.push_back(command);
    for(size_t i=0;i<argc;i++) {
        new_data.push_back(argv[i]);
    }
    bool found = false;
    for(size_t i=0;i<save_data.size();i++) {
        if(save_data[i][0] == command) {
            save_data[i] = new_data;
            found = true;
            break;
        }
    }
    if(!found) save_data.push_back(new_data);
    save_all_changes();
}

void save_all_changes() noexcept {
    if(settings_do_not_save()) return;
    std::ofstream init(std::string(halo_path()) + "\\chimera\\chimerasave.txt");
    if(init.is_open()) {
        init << "###" << std::endl;
        init << "### Chimera build " << CHIMERA_BUILD_STRING << std::endl;
        init << "###" << std::endl;
        init << "### This file is auto-generated. Comments made here may be lost." << std::endl;
        init << "###" << std::endl;
        init << std::endl;
        #ifdef CHIMERA_ALPHA_VERSION
        init << "################################################################################" << std::endl;
        init << "###               WARNING: USE ALPHA BUILDS AT YOUR OWN RISK.                ###" << std::endl;
        init << "################################################################################" << std::endl;
        init << std::endl;
        #endif
        for(size_t i=0;i<save_data.size();i++) {
            auto &entry = save_data[i];
            std::string line = entry[0];
            for(size_t w=1;w<entry.size();w++) {
                line += " \"";
                line += entry[w] + "\"";
            }
            init << line << std::endl;
        }
    }
}

static bool verbose_init = false;

bool read_init_file(const char *path, const char *name) noexcept {
    std::ifstream init(path);
    if(init.is_open()) {
        std::string line;
        size_t ln = 0;
        while(std::getline(init, line)) {
            ln++;
            if(line.size() > 127 || line.size() == 0) {
                continue;
            }
            auto *l = line.data();
            while(*l == ' ') {
                l++;
            }
            line = l;
            size_t length = line.size();
            std::string line2 = line;
            for(size_t i=0;i<length;i++) {
                if(line2[i] == '#') {
                    char linething[128] = {};
                    memcpy(linething, line.data(), i);
                    line2 = linething;
                    break;
                }
            }
            if(line2.size() == 0) {
                continue;
            }
            char x[256] = {};
            switch(execute_chimera_command(line2.data(), !verbose_init, true)) {
                case CHIMERA_COMMAND_ERROR_TOO_MANY_ARGUMENTS: {
                    auto &command = find_chimera_command(line.data());
                    sprintf(x, "%s:%u: Function %s takes at least %u arguments.", name, ln, command.name(), command.max_args());
                    console_out_error(std::string());
                    break;
                }
                case CHIMERA_COMMAND_ERROR_NOT_ENOUGH_ARGUMENTS: {
                    auto &command = find_chimera_command(line.data());
                    sprintf(x, "%s:%u: Function %s takes no more than %u arguments.", name, ln, command.name(), command.min_args());
                    console_out_error(x);
                    break;
                }
                case CHIMERA_COMMAND_ERROR_FAILURE: {
                    auto &command = find_chimera_command(line.data());
                    sprintf(x, "%s:%u: An error occurred while running %s.", name, ln, command.name());
                    console_out_error(x);
                    break;
                }
                case CHIMERA_COMMAND_ERROR_COMMAND_NOT_FOUND: {
                    auto command_name = split_arguments(line.data(), true)[0];
                    sprintf(x, "%s:%u: Function %s was not found.", name, ln, command_name.data());
                    console_out_error(x);
                    break;
                }
                case CHIMERA_COMMAND_ERROR_UNSUPPORTED: {
                    sprintf(x, "%s:%u: The command is not supported right now.", name, ln);
                    console_out_error(x);
                    console_out_error("Either you are not running Halo Custom Edition 1.10, or you are using a mod that conflicts");
                    console_out_error("with this feature.");
                    break;
                }
                case CHIMERA_COMMAND_ERROR_SUCCESS: break;
            }
        }
        return true;
    }
    return false;
}

ChimeraCommandError verbose_init_command(size_t argc, const char **argv) noexcept {
    if(argc == 1) {
        verbose_init = bool_value(argv[0]);
    }
    char x[256] = {};
    sprintf(x, "chimera_verbose_init: %s", verbose_init ? "true" : "false");
    console_out(x);
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
