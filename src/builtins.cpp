#include "builtins.h"
#include "os.h"
#include <algorithm>
#include <filesystem>
#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif
#include <sys/wait.h>

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
                        return;
                }
        }

        void exec(const std::vector<std::string> args) {
                pid_t pid = fork();
                int status; // CAUTION: not initialized

                if (pid == -1) {
                        print("jsh: Error", true);
                        exit(EXIT_FAILURE);
                        return;
                } else if (pid > 0) {
                        waitpid(pid, &status, 0);
                        return;
                } else {
                        std::vector<char*> argv;
                        argv.reserve(args.size() + 1);
                        for (const std::string& s : args) {
                                argv.push_back(const_cast<char*>(s.c_str()));
                        }
                        argv.push_back(nullptr);
                        execvp(argv[0], argv.data());
                        
                        if (errno == ENOENT) {
                                printErr(/*"jsh: " + */std::string(argv[0]) + ": command not found", true); // commented the jsh: out for now, to pass tests
                                _exit(127);
                        } else if (errno == EACCES) {
                                printErr("jsh: " + std::string(argv[0]) + ": permission denied", true);
                                _exit(126);
                        } else {
                                printErr("jsh: " + std::string(argv[0]) + ": exec error", true);
                        }
                        
                        _exit(127);
                } 
        }
}
