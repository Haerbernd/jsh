#ifndef COMPLETION_H
#define COMPLETION_H

namespace jsh {
static void init_path_commands();
static char* cmd_generator(const char* text, int state);
char** completion(const char* text, int start, int end);
void init_completion();
} // namespace jsh

#endif
