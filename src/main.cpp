#include "builtins.h"
#include "completion.h"
#include "os.h"

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include <cstdlib>

#include <readline/history.h>
#include <readline/readline.h>

#ifdef _WIN32
        #include <process.h>
#else
        #include <unistd.h>
#endif

int main() {
        bool running{true};
        
        // Flush after every std::cout / std:cerr
        std::cout << std::unitbuf;
        std::cerr << std::unitbuf;

        rl_bind_key('\t', rl_complete);
        using_history();
        stifle_history(5000); // keep max 5000 entries

        jsh::init_completion();

        std::string histfile = jsh::expandTilde("~/.jsh_history");
        if (!std::filesystem::exists(histfile)) {
                std::ofstream ofs(histfile);
                ofs.close();
        }
        read_history(histfile.c_str());

        while (running) { 
                const char* input_;
                if (!jsh::showCwd) {
                        input_ = readline("$ ");
                } else { 
                        input_ = readline(std::format("[{}]$ ", std::filesystem::current_path().string()).c_str());
                }
                if (!input_) {
                        break; // EOF (Ctrl + D)
                }
                if (input_ && *input_) {
                        add_history(input_);
                }

                std::string input(input_);

                jsh::inputMetaData meta{};

                std::vector<std::string> inputVec = jsh::tokenize(input, &meta);

                if (inputVec[0] == "exit") {
                        running = false;
                        break;
                } else {
                        auto it = jsh::builtins.find(inputVec[0]);
                        if (it != jsh::builtins.end()) {
                                int saved_stdout = dup(STDOUT_FILENO);
                                jsh::apply_redirections(meta);
                                it->second(inputVec);
                                dup2(saved_stdout, STDOUT_FILENO);
                                close(saved_stdout);
                       } else {
                                for (int i{}; i < inputVec.size(); i++) {
                                        inputVec[i] = jsh::expandTilde(inputVec[i]);
                                }
                                jsh::exec(inputVec, meta);
                       }
                } 
        }

        append_history(history_length, histfile.c_str());

        return EXIT_SUCCESS;
}
