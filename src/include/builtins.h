#ifndef BUILTINS_H
#define BUILTINS_H

#include <string>
#include <vector>

namespace jsh {
        void print(std::string msg, bool newline=true);
        void echo(const std::vector<std::string> args);
}

#endif
