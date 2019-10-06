

#ifndef LEXER_RESULT_CPP
#define LEXER_RESULT_CPP

#include "const.cpp"
#include "token.cpp"
#include "stream.cpp"
#include <stdint.h>
#include <unordered_map>
#include <vector>
#include <functional>

namespace parse {

template<typename stream_t>
struct LexerResult {
    typedef Token<stream_t> token_type;
    typedef std::function<void(int64_t&, int64_t&)> report_handler_type;

    LexerCode code;
    std::vector<token_type*> tokens;
    std::unordered_map<std::basic_string<stream_t>, token_type*> table;
    report_handler_type report_handler;

    ~LexerResult() {
        for(auto &token :tokens) {
            delete token;
            token = nullptr;
        }
    };
    
    template<typename T>
    token_type *register_token(const T *token_payload) {
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

    token_type *register_token(TokenType token_type) {
        if (token_type::is_keyword(token_type) || token_type::is_operator(token_type) ||
            token_type::is_mark(token_type)) {
            return new_token(token_type);
        }

        throw std::invalid_argument(std::string("token_type is not type only, which is")
            + stringify(token_type));
    }

    
    token_type *register_token(TokenType token_type, const std::basic_string<stream_t> &buf) {
        if (token_type::is_identifier(token_type)) {
            return register_token(new Identifier<stream_t>(buf));
        }

        if (token_type::is_const_string(token_type)) {
            return register_token(new ConstString<stream_t>(buf));
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

    token_type *set_error(const std::basic_string<stream_t> &buf,
    const std::basic_string<stream_t> &pbuf) {
        return register_token(new ErrorInfo<stream_t>(buf, pbuf));
    }

    void register_report_handler(const report_handler_type &func) {
        report_handler = func;
    }

    friend std::ostream &operator <<(std::ostream &a, const LexerResult &b) {
        a << "LexerResult {\n    code(" << stringify(b.code) << ")\n";
        for(auto &token :b.tokens) {
            a << "    " << token << "\n";
        }
        a << "}";
        return a;
    }
    
    friend std::ostream &operator <<(std::ostream &a, const LexerResult *b) {
        a << *b;
        return a;
    }



private:
    template<typename T>
    token_type *new_token(T payload) {
        auto token = new token_type(payload);
        report_handler(token->line, token->column);
        tokens.push_back(token);
        return token;
    }
};

} // namespace parse

#endif