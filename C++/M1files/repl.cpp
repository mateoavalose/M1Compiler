#include <iostream>
#include <string>
#include "M1files/HeaderFiles/lexer.h"
#include "M1files/HeaderFiles/tokens.h"

#include "M1files/HeaderFiles/repl.h"

Token EOF_TOKEN{TokenType::EOF_TOKEN, ""};
void start_repl() {    
    std::string source;
    while (source != "close()") {
        std::cout << ">> ";
        std::getline(std::cin, source);
        Lexer lexer(source);

        Token token = lexer.next_token();
        while (token.type != TokenType::EOF_TOKEN) {
            Token token = lexer.next_token();
            std::cout << "Type: " << static_cast<int>(token.type) << ", Literal: " << token.literal << std::endl;
        }
    }
}