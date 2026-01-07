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
        std::unordered_map<std::string, BuiltinFn> builtins = {
                {"echo", echo},
                {"type", [](const auto& args) { type(args.at(1)); }},
                {"pwd", [](const auto&) { pwd(); }},
                {"cd", [](const auto& args) {
                        if (args.size() > 1) {
                                cd(expandTilde(args[1]));
                        } else {
                                cd();
                        }
                }},
                {"help", [](const auto&) { help(); }},
                {"cwd", [](const auto&) { toggleCwd(); }},
        };

        bool showCwd = false;

        void echo(const std::vector<std::string> args) {
                std::string msg{};

                for (int i{1}; i < args.size(); i++) {
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

        void exec(const std::vector<std::string>& args, inputMetaData& meta) {
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
                        apply_redirections(meta);

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

        void pwd() {
                print(std::filesystem::current_path().string(), true);
        }

        void cd(std::string newPath) {
                std::filesystem::path path{expandTilde(newPath)}; 

                try {
                        std::filesystem::current_path(path);
                } catch (std::filesystem::filesystem_error& er) {
                        printErr("cd: " + newPath + ": No such file or directory", true);
                }
        }

        void toggleCwd() {
                showCwd = !showCwd;
        }

        void help() {
                inputMetaData meta{};
                exec(std::vector<std::string>{"man", "jsh"}, meta);
        }
}
