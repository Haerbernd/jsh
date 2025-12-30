#include "os.h"
#include <iostream>
#include <sstream>

namespace jsh {
        /*
         * Tokenizes the input string into a string vector. It adheres to common rules regarding special characters.
         * @param input: The input string. Must be a std::string.
         * @return: The tokenized input as a std::string<std::vector>.
         * The input string will be splitted on whitespaces unless each one is escaped with a backslash or they are in a quote.
         * A backslash can escape single and double quotes, whitespaces, backslashes, asterisks (*) and dollar signs. Every other character is treated as normal.
         */
        std::vector<std::string> tokenize(const std::string& input) {
                std::vector<std::string> tokens{};
                std::string current{""};
                bool inSingleQuotes{false};
                bool inDoubleQuotes{false};
                bool backslashEscape{false};
                size_t lastBackslash{};

                for (size_t i{0}; i < input.length(); i++) {
                        char currentChar = input[i];

                        if (backslashEscape && i == lastBackslash + 2) {
                                backslashEscape = false;
                        }

                        if (currentChar == '\\' && !inSingleQuotes && !inDoubleQuotes && !backslashEscape) {
                                backslashEscape = true;
                                lastBackslash = i;
                        } else if (currentChar == '\'' && !inDoubleQuotes && !backslashEscape) {
                                inSingleQuotes = !inSingleQuotes;
                        } else if (currentChar == '"' && !inSingleQuotes && !backslashEscape) {
                                inDoubleQuotes = !inDoubleQuotes;
                        } else if (currentChar == ' ' && (!inSingleQuotes && !inDoubleQuotes && !backslashEscape)) {
                                if (!current.empty()) {
                                        tokens.push_back(current);
                                        current.clear();
                                }
                        } else {
                                current += currentChar;
                        }
                }

                if (!current.empty()) {
                        tokens.push_back(current);
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
}
