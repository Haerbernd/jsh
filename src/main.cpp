#include <iostream>
#include <string>
#include <cstdlib>

int main() {
        bool running{true};

        while (running) {
                // Flush after every std::cout / std:cerr
                std::cout << std::unitbuf;
                std::cerr << std::unitbuf;

                std::cout << "$ ";

                std::string input;
                std::getline(std::cin, input);
                
                if (input == "exit") {
                        running = false;
                        break;
                }
                std::cout << input << ": command not found" << std::endl;
        }

        return EXIT_SUCCESS;
}
