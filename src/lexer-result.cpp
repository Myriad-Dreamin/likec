

#ifndef MYD_LEXER_RESULT_IMPL
#define MYD_LEXER_RESULT_IMPL

#include "lexer-result.h"

namespace parse {

template<typename stream_t>
LexerResult<stream_t>::~LexerResult() {
    for(auto &token :tokens) {
        delete token;
        token = nullptr;
    }
}

template<typename stream_t>
void LexerResult<stream_t>::set_lines(int64_t lines) {
    this->lines = lines;
}
template<typename stream_t>
void LexerResult<stream_t>::set_offset(int64_t offset) {
    this->offset = offset;
}

template<typename stream_t>
template<typename T>
Token<stream_t> *LexerResult<stream_t>::register_token(const T *token_payload) {
    auto token = new_token(token_payload);
    if (table.count(token_payload->name)) {
        auto first_token = table[token_payload->name], last_token = first_token->last();
        token->set_last(last_token);
        first_token->set_last(token);
        return token;
    }
    token->set_last(token);
    table[token_payload->name] = token;
    return token;
}

template<typename stream_t>
Token<stream_t> *LexerResult<stream_t>::register_token(TokenType token_type) {
    if (token_type::is_keyword(token_type) || token_type::is_operator(token_type) ||
        token_type::is_mark(token_type)) {
        return new_token(token_type);
    }

    throw std::invalid_argument(std::string("token_type is not type only, which is")
        + stringify(token_type));
}

template<typename stream_t>
Token<stream_t> *LexerResult<stream_t>::register_token(TokenType token_type, const string &buf) {
    if (token_type::is_identifier(token_type)) {
        return register_token(new Identifier<stream_t>(buf));
    }

    if (token_type::is_const_string(token_type)) {
        return register_token(new ConstString<stream_t>(buf));
    }

    if (token_type::is_space(token_type)) {
        return register_token(new Space<stream_t>(buf));
    }

    if (token_type::is_const_char(token_type)) {
        return register_token(new ConstChar<stream_t>(buf));
    }

    if (token_type::is_comment(token_type)) {
        return register_token(new Comment<stream_t>(buf));
    }

    if (token_type::is_number(token_type)) {
        return register_token(new Number<stream_t>(token_type, buf));
    }

    throw std::invalid_argument(std::string("token_type is type only, which is")
        + stringify(token_type));
}
template<typename stream_t>
Token<stream_t> *LexerResult<stream_t>::set_error(
    const string &buf,
    const string &pbuf
) {
    return register_token(new ErrorInfo<stream_t>(buf, pbuf));
}

template<typename stream_t>
void LexerResult<stream_t>::register_report_handler(const report_handler_type &func) {
    report_handler = func;
}

template<typename stream_t>
std::ostream &operator <<(std::ostream &a, const LexerResult<stream_t> &b) {
    a << "LexerResult {\n    code(" << stringify(b.code) << "), ";
    a << "error(" << std::to_string(b.count[static_cast<raw_token_type>(TokenType::ErrorInfoType)]) << "), ";
    a << "lines(" << std::to_string(b.lines) << "), ";
    a << "characters(" << std::to_string(b.offset) << ")";
    a << "\n";
    for(auto &token :b.tokens) {
        a << "    " << token << "\n";
    }

    for(raw_token_type i = 0; i < TokenCounts; i++) {
        a << "    the count of " << stringify(TokenType(i)) << ": " << std::to_string(b.count[i]) << "\n";
    }
    a << "}";
    return a;
}

template<typename stream_t>
std::ostream &operator <<(std::ostream &a, const LexerResult<stream_t> *b) {
    a << *b;
    return a;
}

template<typename stream_t>
template<typename T>
Token<stream_t> *LexerResult<stream_t>::new_token(T payload) {
    auto token = new Token<stream_t>(payload);
    report_handler(token->line, token->column);
    ++count[static_cast<raw_token_type>(token->token_type)];
    tokens.push_back(token);
    return token;
}

} // namespace parse

#endif