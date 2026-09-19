#include "tokenizer.hpp"

#include <vector>

namespace jsh {
        std::vector<Token> tokenize(std::string& input) {
                bool quot_single{false};
                bool quot_double{false};

                std::string current_string{};
                Token current_token;

                std::vector<Token> tokens{};

                for (char c : input) {
                        if (c == '"') {
                                if (quot_single) {
                                        current_string += c;
                                } else {
                                        quot_double = !quot_double;
                                }
                        } else if (c == '\'') {
                                if (quot_double) {
                                        current_string += c;
                                } else {
                                        quot_single = !quot_single;
                                }
                        } else if (c == ' ') {
                                if (quot_single || quot_double) {
                                        current_string += c;
                                } else {
                                        current_token.type = TOKEN_STRING;
                                        current_token.data.string = current_string;
                                }
                        }
                }
        }
}