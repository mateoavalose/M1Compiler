#include <iostream>
#include <string>

#include "HeaderFiles/lexer.h"
#include "HeaderFiles/tokens.h"
#include "HeaderFiles/repl.h"

Token EndOfFile_TOKEN{TokenType::EOF_TOKEN, ""};
void start_repl() {
    std::string source;
    while (true) {
        std::cout << ">> ";
        std::getline(std::cin, source);
        if (source == "close()") {
            break;
        }

        Lexer lexer(source);
        Token token = lexer.next_token();
        while (token.type != TokenType::EOF_TOKEN) {
            std::cout << "Type: " << enumStr[static_cast<int>(token.type)] << ", Literal: " << token.literal << std::endl;
            token = lexer.next_token();
        }
    }
}