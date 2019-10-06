#include <iostream>
#include "src/lexer.h"
#include "src/lexer-impl.cpp"
#include "src/words-fa.cpp"


using namespace parse;

int main() {
    std::cin >> std::noskipws;
    Lexer<char, 1024> lexer(std::cin);
    auto result = Lexer<char, 1024>::new_result();
    std::cout << lexer.parse(result) << std::endl;
}
