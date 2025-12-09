#include "builtins.h"
#include <algorithm>
#include <iostream>

namespace jsh {
        void print(std::string msg, const bool newline) {
                if (newline) {
                        msg += "\n";
                }
                std::cout << msg;
        }

        void echo(const std::vector<std::string> args) {
                std::string msg{};

                for (int i{0}; i < args.size(); i++) {
                        msg += args[i] + " ";
                }

                print(msg, true);
        }

        void type(const std::string command) {
                if (std::find(BUILTINS.begin(), BUILTINS.end(), command) != BUILTINS.end()) {
                        print(command + " is a shell builtin", true);
                } else {
                        print(command + ": not found", true);
                }
        }
}
