#ifndef BUILTINS_H
#define BUILTINS_H

#include <string>
#include <vector>

namespace jsh {
        const std::vector<std::string> BUILTINS{
                "echo", "exit", "type"
        };

        void print(std::string msg, const bool newline=true);
        void echo(const std::vector<std::string> args);
        void type(const std::string command);
}

#endif
