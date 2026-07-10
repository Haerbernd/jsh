/*
 *  jsh - a bash-inspired linux shell
 *  Copyright (C) 2026  Jan-Hendrik Schmidt
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "builtins.h"
#include "completion.h"
#include "os.h"

#include <cstdlib>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <readline/history.h>
#include <readline/readline.h>
#include <string>

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

int main() {
        bool running{true};

        bool interactive{static_cast<bool>(isatty(STDIN_FILENO))};

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
                std::string input;

                if (interactive) {
                        const char* input_;
                        if (!jsh::showCwd) {
                                input_ = readline("$ ");
                        } else {
                                input_ = readline(
                                    std::format("[{}]$ ",
                                                std::filesystem::current_path()
                                                    .string())
                                        .c_str());
                        }
                        if (!input_) {
                                break; // EOF (Ctrl + D)
                        }
                        if (input_ && *input_) {
                                add_history(input_);
                        }

                        input = input_;

                } else {
                        if (!std::getline(std::cin, input)) {
                                break;
                        }
                }

                jsh::inputMetaData meta{};

                std::vector<std::string> inputVec = jsh::tokenize(input, &meta);

                if (inputVec[0] == "exit") {
                        running = false;
                        break;
                } else {
                        auto it = jsh::builtins.find(inputVec[0]);
                        if (it != jsh::builtins.end()) {
                                int saved_stdout{-1};
                                int saved_stderr{-1};

                                if (meta.redirect_stdout) {
                                        saved_stdout = dup(STDOUT_FILENO);
                                }
                                if (meta.redirect_stderr) {
                                        saved_stderr = dup(STDERR_FILENO);
                                }

                                jsh::apply_redirections(meta);
                                it->second(inputVec);

                                if (saved_stdout != -1) {
                                        dup2(saved_stdout, STDOUT_FILENO);
                                        close(saved_stdout);
                                }
                                if (saved_stderr != -1) {
                                        dup2(saved_stderr, STDERR_FILENO);
                                        close(saved_stderr);
                                }
                        } else {
                                for (int i{}; i < inputVec.size(); i++) {
                                        inputVec[i] =
                                            jsh::expandTilde(inputVec[i]);
                                }
                                jsh::exec(inputVec, meta);
                        }
                }
        }

        append_history(history_length, histfile.c_str());

        return EXIT_SUCCESS;
}
