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

        std::vector<std::string> tokenize(const std::string& input);

        void print(std::string msg, const bool newline=true);
        void printErr(std::string msg, const bool newline=true);
        std::vector<std::string> getPATHDirs();
        bool isExecutable(const std::filesystem::path &p);
        std::string expandTilde(std::string newPath);
}

#endif
