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

#include "os.h"

#include <format>
#include <fstream>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif
#include <fcntl.h>
#include <sys/stat.h>

namespace jsh {
/*
 * Tokenizes the input string into a string vector. It adheres to common rules
 * regarding special characters.
 * @param input: The input string. Must be a std::string.
 * @param meta: A pointer to the inputMetaData struct to synchronize meta data
 * about the current input.
 * @return: The tokenized input as a std::vector<std::string>.
 * The input string will be split on whitespaces unless each one is escaped with
 * a backslash or they are in a quote. A backslash can escape single and double
 * quotes, whitespaces, backslashes. Every other character is treated as normal.
 * > is interpreted as 1> and both redirect STDOUT to a given file. 2> does the
 * same with STDERR, and 0> to STDIN.
 */
std::vector<std::string> tokenize(const std::string& input,
                                  inputMetaData* meta) {
        std::vector<std::string> tokens{};
        std::string current{""};
        bool inSingleQuotes{false};
        bool inDoubleQuotes{false};
        bool backslashEscape{false};
        size_t lastBackslash{};
        bool isOne{false};
        bool isTwo{false};

        for (size_t i{0}; i < input.length(); i++) {
                const char currentChar = input[i];

                if (backslashEscape && i == lastBackslash + 2) {
                        backslashEscape = false;
                }

                if (currentChar == '\\' && !inDoubleQuotes && !inSingleQuotes &&
                    !backslashEscape) {
                        backslashEscape = true;
                        lastBackslash = i;
                } else if ((currentChar == '\\' && inDoubleQuotes) ||
                           (currentChar == '"' && inDoubleQuotes &&
                            backslashEscape)) {
                        if (!backslashEscape) {
                                backslashEscape = true;
                                lastBackslash = i;
                        } else {
                                current += currentChar;
                        }
                } else if (currentChar == '\'' && !inDoubleQuotes &&
                           !backslashEscape) {
                        inSingleQuotes = !inSingleQuotes;
                } else if (currentChar == '"' && !inSingleQuotes &&
                           !backslashEscape) {
                        inDoubleQuotes = !inDoubleQuotes;
                } else if (currentChar == ' ' &&
                           (!inSingleQuotes && !inDoubleQuotes &&
                            !backslashEscape)) {
                        if (isOne) {
                                isOne = false;
                                current += '1';
                        } else if (isTwo) {
                                isTwo = false;
                                current += '2';
                        }

                        if (!current.empty() && !meta->redirect_stdout &&
                            !meta->redirect_stderr) {
                                tokens.push_back(current);
                                current.clear();
                        } else {
                                meta->redirect_file = current;
                        }
                } else if (backslashEscape && inDoubleQuotes) {
                        current += '\\';
                        current += currentChar;
                } else if (currentChar == '>' && !inSingleQuotes &&
                           !inDoubleQuotes && !backslashEscape) {
                        if (isOne) {
                                isOne = false;
                                meta->redirect_stdout = true;
                        } else if (isTwo) {
                                isTwo = false;
                                meta->redirect_stderr = true;
                        } else {
                                meta->redirect_stdout = true;
                        }
                } else if (currentChar == '1' &&
                           (!inSingleQuotes && !inDoubleQuotes &&
                            !backslashEscape)) {
                        if (isOne) {
                                current += '1';
                        }
                        isOne = true;
                } else if (currentChar == '2' &&
                           (!inSingleQuotes && !inDoubleQuotes &&
                            !backslashEscape)) {
                        if (isTwo) {
                                current += '2';
                        }
                        isTwo = true;
                } else {
                        if (isOne) {
                                isOne = false;
                                current += '1';
                        } else if (isTwo) {
                                isTwo = false;
                                current += '2';
                        }

                        current += currentChar;
                }
        }

        if (isOne) {
                current += '1';
        } else if (isTwo) {
                current += '2';
        }

        if (!current.empty() && !meta->redirect_stdout &&
            !meta->redirect_stderr) {
                tokens.push_back(current);
        } else {
                meta->redirect_file = current;
        }

        return tokens;
}

/*
 * Prints the given string. Currently to STDOUT, will maybe be configurable
 * later to other targets (such as a debug file)
 * @param msg: The printed message.
 * @param newline: Whether a newline character (\n) should be appended to the
 * message, default is true.
 */
void print(std::string msg, const bool newline) {
        if (newline) {
                msg += "\n";
        }
        std::cout << msg;
}

/*
 * Prints the given string as an error. Currently to STDERR, will maybe be
 * configurable later to other targets (such as a debug file)
 * @param msg: The printed message.
 * @param newline: Whether a newline character (\n) should be appended to the
 * message, default is true.
 */
void printErr(std::string msg, const bool newline) {
        if (newline) {
                msg += "\n";
        }
        std::cerr << msg;
}

/**/
std::vector<std::string> getPATHDirs() {
        std::vector<std::string> PATHDirs{};
        if (const char* PATHchar = std::getenv("PATH")) {
                const std::string PATH{PATHchar};
                std::stringstream strStream(PATH);
                std::string tempString{};
                while (
                    std::getline(strStream, tempString, PATH_LIST_SEPARATOR)) {
                        PATHDirs.push_back(tempString);
                }
        }
        return PATHDirs;
}

bool isExecutable(const std::filesystem::path& p) {
        const std::filesystem::file_status status{std::filesystem::status(p)};
        if (!std::filesystem::exists(status) or
            !std::filesystem::is_regular_file(status)) {
                return false;
        }

        const std::filesystem::perms perms = status.permissions();

        return (perms & std::filesystem::perms::owner_exec) !=
                   std::filesystem::perms::none or
               (perms & std::filesystem::perms::group_exec) !=
                   std::filesystem::perms::none or
               (perms & std::filesystem::perms::others_exec) !=
                   std::filesystem::perms::none;
}

std::string expandTilde(std::string newPath) {
        if (newPath == "~" || newPath.rfind("~/", 0) == 0) {
                std::string home{};
#ifdef _WIN32
                home = std::getenv("HOMEDRIVE") +
                       std::getenv(
                           "HOMEPATH"); // might just be USERPROFILE instead
#else
                home = std::getenv("HOME");
#endif
                newPath = home + newPath.substr(1);
        }
        return newPath;
}

void writeToFile(const std::string& text, std::string filepath) {
        std::ofstream ofs(filepath);
        if (!ofs) {
                printErr(
                    std::format("{} could not be opened or created for writing",
                                filepath),
                    true);
        }
        ofs << text;
        ofs.close();
}

void apply_redirections(const inputMetaData& meta) {
        if (!meta.redirect_stdout && !meta.redirect_stderr) {
                return;
        }
        int fd = open(meta.redirect_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC,
                      0644);
        if (fd == -1) {
                perror("open");
                _exit(1);
        }
        if (meta.redirect_stdout) {
                dup2(fd, STDOUT_FILENO);
        } else if (meta.redirect_stderr) {
                dup2(fd, STDERR_FILENO);
        }
        close(fd);
}
} // namespace jsh
