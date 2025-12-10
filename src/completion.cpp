#include "completion.h"
#include "builtins.h"
#include "os.h"
#include <filesystem>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <readline/history.h>
#include <readline/readline.h>

namespace jsh {
        // ------------------------------------------------------------
        // 1. Cache all executables from PATH (once at startup)
        // ------------------------------------------------------------
        static std::vector<std::string> PATH_CMDS;

        static void init_path_commands() {
                PATH_CMDS.clear();
                std::vector<std::string> dirs = getPATHDirs();

                for (std::string& d : dirs) {
                        if (!std::filesystem::exists(d)) {
                                continue;
                        }

                        for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(d)) {
                                if (!entry.is_regular_file()) {
                                        continue;
                                }

                                if (isExecutable(entry.path())) {
                                        PATH_CMDS.push_back(entry.path().filename().string());
                                }
                        }
                }
        }

        // ------------------------------------------------------------
        // 2. Generator for command-name completion
        // ------------------------------------------------------------
        static char* cmd_generator(const char* text, int state) {
                static size_t index{0};
                static std::vector<std::string> matches;

                if (state == 0) {
                        index = 0;
                        matches.clear();

                        std::string t(text);

                        // Builtins
                        for (const std::string& b : BUILTINS) {
                                if (b.rfind(t, 0) == 0) {
                                        matches.push_back(b);
                                }
                        }

                        // External commands
                        for (const std::string& cmd : PATH_CMDS) {
                                if (cmd.rfind(t, 0) == 0) {
                                        matches.push_back(cmd);
                                }
                        }
                }

                if (index >= matches.size()) {
                        return nullptr;
                }

                return strdup(matches[index++].c_str());
        }

        // ------------------------------------------------------------
        // 3. Main completion dispatcher
        // ------------------------------------------------------------
        char** completion(const char* text, int start, int end) {
                (void)end; // ununsed but must exist
                
                // If cursor is at the beginning (completing first token) -> command names
                if (start == 0) {
                        return rl_completion_matches(text, cmd_generator);
                }

                // Otherwise -> default to file completion
                return nullptr;
        }

        
        // ------------------------------------------------------------
        // 4. Initialization function to call once at startup
        // ------------------------------------------------------------
        void init_completion() {
                init_path_commands();
                rl_attempted_completion_function = completion;
        }
}
