#include "os.h"

#include <string>
#include <vector>

#include <cassert>

using jsh::inputMetaData;
using jsh::tokenize;

int main() {
        {
                inputMetaData meta{};
                auto tokens = tokenize("echo hello world", &meta);

                assert(tokens.size() == 3);
                assert(tokens[0] == "echo");
                assert(tokens[1] == "hello");
                assert(tokens[2] == "world");
                assert(!meta.redirect_stdout);
                assert(!meta.redirect_stderr);
        }
        {
                inputMetaData meta{};
                auto tokens = tokenize("echo hi > out.txt", &meta);

                assert(tokens.size() == 2);
                assert(tokens[1] == "hi");
                assert(meta.redirect_stdout);
                assert(meta.redirect_file == "out.txt");
        }

        return 0;
}
