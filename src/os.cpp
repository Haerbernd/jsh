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
         * @return: The tokenized input as a std::vector<std::string>.
         * The input string will be splitted on whitespaces unless each one is escaped with a backslash or they are in a quote.
         * A backslash can escape single and double quotes, whitespaces, backslashes. Every other character is treated as normal.
         * > is interpreted as 1> and both redirect STDOUT to a given file. 2> does the same with STDERR, and 0> to STDIN.
         */
        std::vector<std::string> tokenize(const std::string& input, inputMetaData* meta) {
                std::vector<std::string> tokens{};
                std::string current{""};
                bool inSingleQuotes{false};
                bool inDoubleQuotes{false};
                bool backslashEscape{false};
                size_t lastBackslash{};
                bool isOne{false};
                bool isTwo{false};

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
                                if (isOne) {
                                        isOne = false;
                                        current += '1';
                                } else if (isTwo) {
                                        isTwo = false;
                                        current += '2';
                                }

                                if (!current.empty() && !meta->redirect_stdout && !meta->redirect_stderr) {
                                        tokens.push_back(current);
                                        current.clear();
                                } else {
                                        meta->redirect_file = current;
                                }
                        } else if (backslashEscape && inDoubleQuotes) {
                                current += '\\';
                                current += currentChar;
                        } else if (currentChar == '>' && !inSingleQuotes && !inDoubleQuotes && !backslashEscape) {
                                if (isOne) {
                                        isOne = false;
                                        meta->redirect_stdout = true;
                                } else if (isTwo) {
                                        isTwo = false;
                                        meta->redirect_stderr = true;
                                } else {
                                        meta->redirect_stdout = true;
                                }
                        } else if (currentChar == '1' && (!inSingleQuotes && !inDoubleQuotes && !backslashEscape)) {
                                if (isOne) {
                                        current += '1';
                                }
                                isOne = true;
                        } else if (currentChar == '2' && (!inSingleQuotes && !inDoubleQuotes && !backslashEscape)) {
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

                if (!current.empty() && !meta->redirect_stdout && !meta->redirect_stderr) {
                        tokens.push_back(current);
                } else {
                        meta->redirect_file = current;
                }

                return tokens;
        }

        /*
         * Prints the given string. Currently to STDOUT, will maybe be configurable later to other targets (such as a debug file)
         * @param msg: The printed message.
         * @param newline: Whether a newline character (\n) should be appended to the message, default is true.
         */
        void print(std::string msg, const bool newline) {
                if (newline) {
                        msg += "\n";
                }
                std::cout << msg;
        }

        /*
         * Prints the given string as an error. Currently to STDERR, will maybe be configurable later to other targets (such as a debug file)
         * @param msg: The printed message.
         * @param newline: Whether a newline character (\n) should be appended to the message, default is true.
         */
        void printErr(std::string msg, const bool newline) {
                if (newline) {
                        msg += "\n";
                }
                std::cerr << msg;
        }

        /*
         * Returns the directories on PATH as a std::vector<std::string>.
         * return: std::vector<std::string> directories on PATH. Each directory has is its own item in the std::vector.
         * */
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

        /*
         * Will check whether a file/path is executable (by anyone).
         * @param p: The path that will be checked for execute permissions.
         * @return: Returns true when any combination of the following are true: owner has execute permission, group has execute permission, others (all) have execute permission
         * */
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

        /*
         * Will expand a tilde at the beginning of a given path to the users home directory or do nothing if no tilde is found.
         * @param path: The path that should be checked for tilde and expanded (if a tilde is found at the beginning of the path.
         * @return: The new path with the users home instead of the tilde (at the beginning) or the given path if no tilde was found at the beginning of the string.
         * */
        std::string expandTilde(std::string path) {
                if (path == "~" || path.rfind("~/", 0) == 0) {
                        std::string home{};
                        #ifdef _WIN32
                        home = std::getenv("HOMEDRIVE") + std::getenv("HOMEPATH"); // might just be USERPROFILE instead
                        #else
                        home = std::getenv("HOME");
                        #endif
                        std::string newPath {home + path.substr(1)};
                        return newPath;
                }
                return path;
        }

        /*
         * Writes a given string to a given file (creates the file if it does not exist). The string will be appended to the file, data already in the file will not be overwritten. It is not necessary to prepend a \n to enforce a newline for the text.
         * @param text: The text that should be written to file.
         * @param filepath: The file(path) to which the text shall be written.
         * */
        void writeToFile(std::string text, std::string filepath) {
                std::ofstream ofs(filepath);
                if (!ofs) {
                        printErr(std::format("{} could not be opened or created for writing", filepath), true);
                }
                ofs << text;
                ofs.close();
        }

        /*
         * Applies the wanted redirections to the process's file descriptors (e.g. append STDOUT to a file).
         * @param meta: A reference to the inputMetaData struct/object that holds the information about the redirections that should be applied.
         * */
        void apply_redirections(const inputMetaData& meta) {
                if (!meta.redirect_stdout && !meta.redirect_stderr) {
                        return;
                }
                int fd = open(meta.redirect_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
}
