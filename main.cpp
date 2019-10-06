#include <iostream>
#include <fstream>
#include "src/lexer.h"
#include "src/lexer-impl.cpp"
#include "src/words-fa.cpp"
#define finally 


using namespace parse;

int main() {
    // std::cin >> std::noskipws;
    std::fstream streamX;
    streamX.open("test.cpp", std::ios::in);
    streamX.seekg(0, std::ios::beg);
    try {
        Lexer<char, 1024> lexer(streamX);
        auto result = Lexer<char, 1024>::new_result();
        std::cout << lexer.parse(result) << std::endl;
    } catch (std::exception &e) {
        std::cout << "catch " << e.what() << std::endl;
    } finally {
        streamX.close();
    }
}
