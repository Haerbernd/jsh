#ifndef JSH_TOKENIZER_HPP
#define JSH_TOKENIZER_HPP

#include <string>
#include <vector>

namespace jsh {
typedef enum TokenType {
        TOKEN_STRING,
        TOKEN_AMPERSAND_SINGLE,
        TOKEN_AMPERSAND_DOUBLE
} TokenType;

typedef struct Token {
        TokenType type;
        union data {
                std::string string;
                bool amp_single;
                bool amp_double;
        } data;
} Token;

std::vector<Token> tokenize(std::string& input);
} // namespace jsh

#endif
