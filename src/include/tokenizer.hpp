#ifndef JSH_TOKENIZER_HPP
#define JSH_TOKENIZER_HPP

#include <string>
#include <vector>

namespace jsh {
typedef enum TokenType { STRING, AMPERCENT_SINGLE, AMPERCENT_DOUBLE } TokenType;

typedef struct Token {
        TokenType type;
        union data {
                std::string string;
                bool amp_single;
                bool amp_double;
        };
} Token;

std::vector<Token> tokenize(std::string& input);
} // namespace jsh

#endif
