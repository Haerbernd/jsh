#ifndef OS_H
#define OS_H

#include <filesystem>
#include <string>
#include <vector>

namespace jsh {

        #ifdef _WIN32
                constexpr char PATH_LIST_SEPARATOR{';'};
        #else
                constexpr char PATH_LIST_SEPARATOR{':'};
        #endif

        void print(std::string msg, const bool newline=true);
        std::vector<std::string> getPATHDirs();
        bool isExecutable(const std::filesystem::path &p);
}

#endif
