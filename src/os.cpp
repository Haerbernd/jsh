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
         * Tokenizes the input string into a string vector. It adheres to common rules regarding special characters.
         * @param input: The input string. Must be a std::string.
         * @param meta: A pointer to the inputMetaData struct to synchronize meta data about the current input.
         * @return: The tokenized input as a std::string<std::vector>.
         * The input string will be splitted on whitespaces unless each one is escaped with a backslash or they are in a quote.
         * A backslash can escape single and double quotes, whitespaces, backslashes, asterisks (*) and dollar signs. Every other character is treated as normal.
         */
        std::vector<std::string> tokenize(const std::string& input, inputMetaData* meta) {
                std::vector<std::string> tokens{};
                std::string current{""};
                bool inSingleQuotes{false};
                bool inDoubleQuotes{false};
                bool backslashEscape{false};
                size_t lastBackslash{};
                bool isOne{false};

                for (size_t i{0}; i < input.length(); i++) {
                        char currentChar = input[i];

                        if (backslashEscape && i == lastBackslash + 2) {
                                backslashEscape = false;
                        }

                        if (currentChar == '\\' && !inDoubleQuotes && !inSingleQuotes && !backslashEscape) {
                                backslashEscape = true;
                                lastBackslash = i;
                        } else if ((currentChar == '\\' && inDoubleQuotes) || (currentChar == '"' && inDoubleQuotes && backslashEscape)) {
                                if (!backslashEscape) {
                                        backslashEscape = true;
                                        lastBackslash = i;
                                } else {
                                        current += currentChar;
                                }
                        } else if (currentChar == '\'' && !inDoubleQuotes && !backslashEscape) {
                                inSingleQuotes = !inSingleQuotes;
                        } else if (currentChar == '"' && !inSingleQuotes && !backslashEscape) {
                                inDoubleQuotes = !inDoubleQuotes;
                        } else if (currentChar == ' ' && (!inSingleQuotes && !inDoubleQuotes && !backslashEscape)) {
                                if (!current.empty() && !meta->outputRedirection) {
                                        tokens.push_back(current);
                                        current.clear();
                                } else {
                                        meta->outputRedirectionFile = current;
                                }
                        } else if (backslashEscape && inDoubleQuotes) {
                                current += '\\';
                                current += currentChar;
                        } else if (currentChar == '>' && !inSingleQuotes && !inDoubleQuotes && !backslashEscape) {
                                if (isOne) {
                                        isOne = false;
                                }

                                meta->outputRedirection = true;
                        } else if (currentChar == '1' && !inSingleQuotes && !inDoubleQuotes && !backslashEscape) {
                                isOne = true;
                        } else {
                                if (isOne) {
                                        isOne = false;
                                        current += '1';
                                }

                                current += currentChar;
                        }
                }

                if (!current.empty() && !meta->outputRedirection) {
                        tokens.push_back(current);
                } else {
                        meta->outputRedirectionFile = current;
                }

                return tokens;
        }

        void print(std::string msg, const bool newline) {
                if (newline) {
                        msg += "\n";
                }
                std::cout << msg;
        }

        void printErr(std::string msg, const bool newline) {
                if (newline) {
                        msg += "\n";
                }
                std::cerr << msg;
        }

        std::vector<std::string> getPATHDirs() {
                std::vector<std::string> PATHDirs{};
                if (const char* PATHchar = std::getenv("PATH")) {
                        std::string PATH{PATHchar};
                        std::stringstream strStream(PATH);
                        std::string tempString{};
                        while (std::getline(strStream, tempString, PATH_LIST_SEPARATOR)) {
                                PATHDirs.push_back(tempString);
                        }
                }
                return PATHDirs;
        }

        bool isExecutable(const std::filesystem::path &p) {
                auto status{std::filesystem::status(p)};
                if (!std::filesystem::exists(status) or !std::filesystem::is_regular_file(status)) {
                        return false;
                }

                std::filesystem::perms perms = status.permissions();

                return (perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none or
                       (perms & std::filesystem::perms::group_exec) != std::filesystem::perms::none or
                       (perms & std::filesystem::perms::others_exec) != std::filesystem::perms::none;
        }

        std::string expandTilde(std::string newPath) {
                if (newPath == "~" || newPath.rfind("~/", 0) == 0) {
                        std::string home{};
                        #ifdef _WIN32
                        home = std::getenv("HOMEDRIVE") + std::getenv("HOMEPATH"); // might just be USERPROFILE instead
                        #else
                        home = std::getenv("HOME");
                        #endif
                        newPath = home + newPath.substr(1);
                }
                return newPath;
        }

        void writeToFile(std::string text, std::string filepath) {
                std::ofstream ofs(filepath);
                if (!ofs) {
                        printErr(std::format("{} could not be opened or created for writing", filepath), true);
                }
                ofs << text;
                ofs.close();
        }

        void apply_redirections(const inputMetaData& meta) {
                if (meta.outputRedirection) {
                        int fd = open(meta.outputRedirectionFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        if (fd == -1) {
                                perror("open");
                                _exit(1);
                        }
                
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                }
        }
}
