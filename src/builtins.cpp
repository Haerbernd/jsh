#include "builtins.h"
#include "os.h"
#include <algorithm>
#include <filesystem>

namespace jsh {
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
                        std::vector<std::string> PATHDirs{getPATHDirs()};
                       
                        

                        std::filesystem::path currentPath{};
                        std::string currentPathStr{};
                        std::vector<std::string> files{};
                        for (int i{}; i < PATHDirs.size(); i++) {
                                currentPathStr = PATHDirs[i] + "/" + command;
                                currentPath = currentPathStr;
                                if (isExecutable(currentPath)) {
                                        print(command + " is " + currentPathStr, true);
                                        return;
                                }
                        
                        }

                        print(command + ": not found", true);
                }
        }
}
