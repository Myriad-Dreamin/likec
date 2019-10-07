
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

    //临时保存的字符串结果缓冲区
    string buf;
    //输入流
    Stream program;
    //LL(1)探针
    stream_t nextToken;
    //词法分析结果
    result_type *result;

public:
    Lexer(istream &in);

    static result_type *new_result();
    result_type *parse(result_type *result);
    
    bool parseMark();
    bool parseSpace();
    bool parseNumber();
    bool parseComment();
    bool parseKeywords();
    bool parseOperator();
    bool parseConstChar();
    bool parseIdentifier();
    bool parseConstString();

    bool emplaceDot();

private:
    void reclaim(const string &s);
    void set_buf(const string &s);
};

}

#endif
