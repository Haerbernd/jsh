#include "os.h"

#include <fstream>

#include <cassert>
#include <fcntl.h>
#include <unistd.h>

using jsh::inputMetaData;
using jsh::apply_redirections;

int main() {
        inputMetaData meta{};
        meta.redirect_stdout = true;
        meta.redirect_file = "redir_test.txt";

        int saved = dup(STDOUT_FILENO);
        apply_redirections(meta);

        write(STDOUT_FILENO, "hello\n", 6);

        dup2(saved, STDOUT_FILENO);
        close(saved);

        std::ifstream ifs("redir_test.txt");
        std::string content;
        std::getline(ifs, content);

        assert(content == "hello");

        unlink("redir_test.txt");
        return 0;
}
