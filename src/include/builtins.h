#ifndef BUILTINS_H
#define BUILTINS_H

#include "os.h"

#include <functional>
#include <unordered_map>
#include <string>
#include <vector>

namespace jsh {
        using BuiltinFn = std::function<void(const std::vector<std::string>&)>;

        extern std::unordered_map<std::string, BuiltinFn> builtins;

        const std::vector<std::string> BUILTINS {
                "echo", "exit", "type", "pwd", "cd", "help", "cwd"
        }; 

        extern bool showCwd;

        void echo(std::vector<std::string> args);
        void type(std::string command);
        void exec(const std::vector<std::string>& args, const inputMetaData& meta);
        void pwd();
        void cd(const std::string& newPath="~");
        void toggleCwd();
        void help();
}

#endif
