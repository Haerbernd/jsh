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

#include "completion.h"

#include "builtins.h"
#include "os.h"

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <readline/history.h>
#include <readline/readline.h>
#include <string>
#include <vector>

namespace jsh {
static std::vector<std::string> PATH_CMDS;

static void init_path_commands() {
        PATH_CMDS.clear();
        std::vector<std::string> dirs = getPATHDirs();

        for (std::string& d : dirs) {
                if (!std::filesystem::exists(d)) {
                        continue;
                }

                for (const std::filesystem::directory_entry& entry :
                     std::filesystem::directory_iterator(d)) {
                        if (!entry.is_regular_file()) {
                                continue;
                        }

                        if (isExecutable(entry.path())) {
                                PATH_CMDS.push_back(
                                    entry.path().filename().string());
                        }
                }
        }
}

static char* cmd_generator(const char* text, const int state) {
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

char** completion(const char* text, const int start, const int end) {
        (void)end; // unused but must exist

        // If cursor is at the beginning (completing first token) -> command
        // names
        if (start == 0) {
                return rl_completion_matches(text, cmd_generator);
        }

        // Otherwise -> default to file completion
        return nullptr;
}

void init_completion() {
        init_path_commands();
        rl_attempted_completion_function = completion;
}
} // namespace jsh
