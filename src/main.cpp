#include "builtins.h"
#include "completion.h"
#include "os.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <readline/history.h>
#include <readline/readline.h>

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
        read_history(histfile.c_str());

        while (running) { 
                const char* input_ = readline("$ ");
                if (!input_) {
                        break; // EOF (Ctrl + D)
                }
                if (input_ && *input_) {
                        add_history(input_);
                }

                std::string input(input_);

                std::vector<std::string> inputVec = jsh::tokenize(input);

                if (inputVec[0] == "exit") {
                        running = false;
                        break;
                } else if (inputVec[0] == "echo") {
                        std::vector<std::string> echoVec{};
                        for (int i{1}; i < inputVec.size(); i++) {
                                echoVec.push_back(inputVec[i]);
                        }
                        jsh::echo(echoVec);
                } else if (inputVec[0] == "type") {
                        if (inputVec.size() > 1) {
                                jsh::type(inputVec[1]); // return value is ignored
                        }
                } else if (inputVec[0] == "pwd") {
                        jsh::pwd();
                } else if (inputVec[0] == "cd") {
                        jsh::cd(jsh::expandTilde(inputVec[1]));
                } else {
                        for (int i{}; i < inputVec.size(); i++) {
                                inputVec[i] = jsh::expandTilde(inputVec[i]);
                        }
                        jsh::exec(inputVec);
                }
        }

        append_history(history_length, histfile.c_str());

        return EXIT_SUCCESS;
}
