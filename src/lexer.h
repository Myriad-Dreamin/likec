
#ifndef MYD_LEXER_H
#define MYD_LEXER_H

#include "definer.h"
#include <iostream>

namespace parse {

template<typename stream_t>
class LexerResult;

template<typename stream_t, int64_t buffer_size, class Stream, bool unread_flag>
class Lexer {
private:
    using istream = std::basic_istream<stream_t>;
    using result_type = LexerResult<stream_t>;
    using string = std::basic_string<stream_t>;

    stream_t nextToken;
    Stream program;
    string buf;
    result_type *result;

public:
    Lexer(istream &in);

    static result_type *new_result();
    result_type *parse(result_type *result);

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
    void reclaim(const string &s);
    void set_buf(const string &s);
};

}

#endif
