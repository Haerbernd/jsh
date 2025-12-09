#include "builtins.h"
#include <iostream>
#include <string>
#include <sstream>
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

                std::stringstream strStream(input);
                std::string tempString{};
                std::vector<std::string> inputVec{};
                const char inputDelimiter{' '};

                while(std::getline(strStream, tempString, inputDelimiter)) {
                        inputVec.push_back(tempString);
                }

                if (inputVec[0] == "exit") {
                        running = false;
                        break;
                } else if (inputVec[0] == "echo") {
                        std::vector<std::string> echoVec{};
                        for (int i{1}; i < inputVec.size(); i++) {
                                echoVec.push_back(inputVec[i]);
                        }
                        jsh::echo(echoVec);
                } else if (inputVec[0] == "type") {
                        if (inputVec.size() > 1) {
                                jsh::type(inputVec[1]);
                        }
                } else {
                        std::cout << input << ": command not found" << std::endl;
                }
        }

        return EXIT_SUCCESS;
}
