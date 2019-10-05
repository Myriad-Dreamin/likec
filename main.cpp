#include <iostream>
#include "src/lexer.h"
#include "src/lexer-impl.cpp"

using namespace parse;

int main() {
    std::cin>> std::noskipws;
    Lexer<char, 1024> lexer(std::cin);
    std::cout << lexer.parse();
}
