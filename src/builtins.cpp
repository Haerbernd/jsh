#include "builtins.h"
#include <iostream>

namespace jsh {
        void print(std::string msg, bool newline) {
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
}
