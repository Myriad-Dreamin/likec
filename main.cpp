#include <iostream>
#include <fstream>
#include <cstring>
#include "src/export.h"
#include "src/svdx.h"
#define finally 

void print_help() {
    std::cout <<
    "prog <source|-s> [-o <target>] \n"
    "    <target>: the file to output\n"
    "    <source>: the file to lex\n"
    "    -s: use stdin from console\n";
    exit(0);
}

int main(int argc, char *argv[]) {
    const char *output_path = nullptr, *input_path = nullptr;
    if (argc == 1) {
        print_help();
    }
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-o")) {
            if (output_path != nullptr) {
                print_help();
            }
            if (i == argc - 1) {
                print_help();
            } else {
                output_path = argv[++i];
            }

            continue;
        }

        if (input_path == nullptr) {
            input_path = argv[i];
        } else {
            print_help();
        }
    }

    // std::cin >> std::noskipws;
    std::istream *streamX = &std::cin;
    std::ostream *streamY = &std::cout;
    std::ofstream streamYf;
    std::ifstream streamXf;
    if (output_path != nullptr) {
        streamYf.open(output_path);
        streamY = &streamYf;
    }
    if (input_path != nullptr) {
        streamXf.open(input_path);
        streamX = &streamXf;
    }

    try {
        using LexerInstanceClass = parse::basic_lexer<1024, false>;

        LexerInstanceClass lexer(*streamX);
        auto result = LexerInstanceClass::new_result();
        *streamY << lexer.parse(result) << std::endl;
    } catch (std::exception &e) {
        std::cout << "catch " << e.what() << std::endl;
    } finally {
        if (input_path) streamXf.close();
        if (output_path) streamYf.close();
    }
}
