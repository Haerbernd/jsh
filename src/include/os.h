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

        struct inputMetaData{
                bool redirect_stdout{false};
                bool redirect_stderr{false};
                bool redirect_stdin{false};   // not implemented for now
                std::string redirect_file{""};
        };

        std::vector<std::string> tokenize(const std::string& input, inputMetaData* meta);

        void print(std::string msg, const bool newline=true);
        void printErr(std::string msg, const bool newline=true);
        std::vector<std::string> getPATHDirs();
        bool isExecutable(const std::filesystem::path &p);
        std::string expandTilde(std::string newPath);
        void writeToFile(std::string text, std::string filepath);
        void apply_redirections(const inputMetaData& meta);
}

#endif
