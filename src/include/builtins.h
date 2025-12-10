#ifndef BUILTINS_H
#define BUILTINS_H

#include <string>
#include <vector>

namespace jsh {
        const std::vector<std::string> BUILTINS{
                "echo", "exit", "type", "pwd"
        };

        void echo(const std::vector<std::string> args);
        void type(const std::string command);
        void exec(const std::vector<std::string> args);
        void pwd();
}

#endif
