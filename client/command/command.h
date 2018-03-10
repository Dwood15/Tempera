#pragma once

#include <string>
#include <vector>

enum ChimeraCommandError {
    /// The command was executed successfully.
    CHIMERA_COMMAND_ERROR_SUCCESS,

    /// The command was executed but failed. An error may have been printed to the console.
    CHIMERA_COMMAND_ERROR_FAILURE,

    /// The number of arguments specified was greater than ChimeraCommand::i_max_args.
    CHIMERA_COMMAND_ERROR_TOO_MANY_ARGUMENTS,

    /// The number of arguments specified was less than ChimeraCommand::i_min_args.
    CHIMERA_COMMAND_ERROR_NOT_ENOUGH_ARGUMENTS,

    /// The command cannot be used at this time, possibly due to a missing signature.
    CHIMERA_COMMAND_ERROR_UNSUPPORTED,

    /// The command was not found or the commands list was not initialized.
    CHIMERA_COMMAND_ERROR_COMMAND_NOT_FOUND,
};

/// This is a pointer to a function executed when ChimeraCommand::execute() is called.
///
/// argc is guaranteed to always be within ChimeraCommand::i_min_args and ChimeraCommand::i_max_args.
///
/// Dereferencing argv while argc is 0 is undefined behavior.
typedef ChimeraCommandError (*ChimeraCommandFunctionPointer)(size_t argc, const char **argv);

class ChimeraCommand {
private:
    /// This stores the name of the command.
    std::string i_name;

    /// This stores the category of the command.
    std::string i_category;

    /// This stores the help information of the command.
    std::string i_help;

    /// This is whether or not the function is valid and can be used.
    bool i_supported;

    /// This is whether or not the function saves.
    bool i_saves;

    /// This is a pointer to the function.
    ChimeraCommandFunctionPointer i_function;

    /// This is the minimum number of arguments required to execute the command.
    size_t i_min_args;

    /// This is the maximum number of arguments required to execute the command.
    size_t i_max_args;
public:
    /// Return the name of the function.
    const char *name() const noexcept;

    /// Return the category of the function.
    const char *category() const noexcept;

    /// Get the help information for the function.
    const char *help() const noexcept;

    /// Get whether or not this command will function.
    bool supported() const noexcept;

    /// Get whether or not this command should save in settings.
    bool saves() const noexcept;

    /// Attempt to execute the function.
    ///
    /// If argc is less than ChimeraCommand::i_min_args or greater than ChimeraCommand::i_max_args then the function,
    /// will not be executed.
    ChimeraCommandError execute(int argc=0, const char **argv=nullptr) const noexcept;

    /// Return the minimum arguments that can be used for this function.
    size_t min_args() const noexcept;

    /// Return the maximum arguments that can be used for this function.
    size_t max_args() const noexcept;

    /// Instantiate a ChimeraCommand using the specified name, function, and (optionally) the range of arguments.
    ///
    /// By default, min_args and max_args are set to 0. If max_args is 0 or unspecified, then it will be set to
    /// min_args. An exception will be thrown if max_args is non-zero and is less than min_args.
    ChimeraCommand(const char *name, ChimeraCommandFunctionPointer function, const char *category=nullptr, const char *help=nullptr, size_t min_args=0, size_t max_args=0, bool supported=true, bool saves=false);
};

/// Split text into arguments.
std::vector<std::string> split_arguments(const char *input, bool lowercase_first_argument=false);

/// Search for and execute the Chimera command.
ChimeraCommandError execute_chimera_command(const char *command_input, bool silently=false, bool save=true);

/// Return the Chimera command that would be executed if command_input was given.
ChimeraCommand &find_chimera_command(const char *command_input);

/// Return true if v is equal to "1" or "true".
bool bool_value(const char *v) noexcept;

/// Function for command chimera
ChimeraCommandError chimera_command(size_t argc, const char **argv) noexcept;
