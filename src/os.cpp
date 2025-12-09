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
                std::string cwd{std::filesystem::current_path()};
                PATHDirs.push_back(cwd);
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
