

#ifndef LEXER_H
#define LEXER_H

#include "const.cpp"
#include "lexer-result.cpp"
#include "stream.cpp"
#include <iostream>
#include <stdint.h>

namespace parse {

template<typename stream_t, int64_t buffer_size>
class Lexer {
private:
    stream_t nextToken;
    Stream<stream_t, buffer_size> program;
    LexerResult<stream_t> *result;

public:
    
    Lexer(std::istream &in) : program(&in) {
        this->nextToken = program.Read();
    }

    static LexerResult<stream_t> *new_result();
    LexerResult<stream_t> *parse(LexerResult<stream_t> *result);
    bool parseKeywords();
    bool parseOperator();
    bool parseComment();
    bool parseMark();
    bool parseIdentifier();
    bool parseNumber();
    bool parseSpace();
    bool parseConstChar();
    bool parseConstString();
private:
    void reclaim(const std::basic_string<stream_t> &s);
};
}
#endif


