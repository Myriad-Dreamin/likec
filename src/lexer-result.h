
#ifndef MYD_LEXER_RESULT_H
#define MYD_LEXER_RESULT_H

#include "definer.h"
#include "token.cpp"
#include <unordered_map>
#include <vector>
#include <functional>

namespace parse {


template<typename stream_t>
struct LexerResult {
    using token_type = Token<stream_t>;
    using string = std::basic_string<stream_t>;
    using report_handler_type = std::function<void(int64_t&, int64_t&)>;

    LexerCode code;
    int64_t lines, offset;
    std::vector<token_type*> tokens;
    int64_t count[TokenCounts];
    std::unordered_map<string, token_type*> table;
    report_handler_type report_handler;

    ~LexerResult();
    void set_lines(int64_t lines);
    void set_offset(int64_t offset);
    void register_report_handler(const report_handler_type &func);
    
    template<typename T>
    token_type *register_token(const T *token_payload);
    token_type *register_token(TokenType token_type);
    token_type *register_token(TokenType token_type, const string &buf);
    token_type *set_error(
        const string &buf,
        const string &pbuf
    );

    template<typename _stream_t>
    friend std::ostream &operator<<(std::ostream &a, const LexerResult<_stream_t> &b);
    template<typename _stream_t>
    friend std::ostream &operator<<(std::ostream &a, const LexerResult<_stream_t> *b);

private:
    template<typename T>
    token_type *new_token(T payload);
};

}

#endif
