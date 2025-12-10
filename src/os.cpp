#include "os.h"
#include <iostream>
#include <sstream>

namespace jsh {
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
}
