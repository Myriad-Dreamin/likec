

#ifndef LEXER_H
#define LEXER_H

#include "const.cpp"
#include <iostream>
#include <queue>
#include <stdint.h>

namespace parse {

    enum LexerCode: uint16_t {
        OK,
        NotMatch,
    };

    const char *stringify(const LexerCode &c) {
        switch (c)
        {
        case LexerCode::OK:
            return "OK";
        case LexerCode::NotMatch:
            return "NotMatch";
        default:
            throw("not possible");
        }
    }
    
    struct Identifier {
        const std::string name;
        const int64_t line, column;
    };


    class Token {
        TokenType token_type;
        void* token_reference;
    public:
        Token(TokenType token_type) {
            token_reference = nullptr;
            this->token_type = TokenType::ErrorType;

            if (TokenType::KeywordBegin <= token_type && token_type < TokenType::KeywordEnd) {
                this->token_type = token_type;
            }
            if (TokenType::OperatorBegin <= token_type && token_type < TokenType::OperatorEnd) {
                this->token_type = token_type;
            }
        }
        Token(TokenType token_type, const struct Identifier *Identifier) {
            token_reference = nullptr;
            this->token_type = TokenType::ErrorType;

            if (token_type == TokenType::Identifier) {
                this->token_type = token_type;
                token_reference = reinterpret_cast<void*>(const_cast<struct Identifier *>(Identifier));
            }
        }

        ~Token() {
            this->token_type = TokenType::ErrorType;
            token_reference = nullptr;
        }

        const std::string &as_keyword() {
            if (is_keyword()) {
                return _key_words[token_type - TokenType::KeywordBegin];
            }
            return _error_type;
        };

        bool is_keyword() {
            return TokenType::KeywordBegin <= token_type && token_type < TokenType::KeywordEnd;
        }

        const std::string &as_operator() {
            if (is_keyword()) {
                return _operators[token_type - TokenType::OperatorBegin];
            }
            return _error_type;
        };

        bool is_operator() {
            return TokenType::OperatorBegin <= token_type && token_type < TokenType::OperatorEnd;
        }
        
        const struct Identifier *as_identifier() {
            if (is_identifier()) {
                return reinterpret_cast<const struct Identifier *>(token_reference);
            }
            return nullptr;
        };

        bool is_identifier() {
            return TokenType::Identifier == token_type;
        }

    };

    struct LexerResult {
        LexerCode code;
        std::vector<Token> tokens;

        friend std::ostream &operator <<(std::ostream &a, const LexerResult &b) {
            a << "LexerResult {\n    code(" << stringify(b.code) << ")\n}";
            return a;
        };
    };

    template<typename stream_t, int64_t buffer_size>
    class Stream {
        typedef bool even_check[-(buffer_size & 1)];
        typedef bool zero_check[buffer_size - 1];

        const size_t stream_unit_size = sizeof(stream_t);
        const size_t half_buffer_size = buffer_size >> 1;

        std::istream *in;
        std::deque<stream_t> buffer;
        
        public:
        Stream(std::istream *in) {
            this->in = in;
            this->in->width(stream_unit_size);
        };
        Stream(const Stream &s) { throw std::exception("do not copy stream"); };
        Stream(const Stream &&s) {
            this->buffer = std::move(s.buffer);
            this->in = in;
            s.in = nullptr;
        };

        stream_t Read() {
            fit(buffer);
            return last(buffer);
        }

        void Unread(const stream_t &bit) {
            buffer.push_front(bit);
            fit(buffer);
        }

        private:
        inline void shrink(std::deque<stream_t> &buffer) {
            if (buffer.size() > buffer_size) {
                in->seekg((buffer.size() - buffer_size) * stream_unit_size, std::ios::cur);
                while (buffer.size() > buffer_size) {
                    buffer.pop_front();
                }
            }
        }
   
        inline void fill(std::deque<stream_t> &buffer) {
            stream_t bit;
            while (buffer.size() < half_buffer_size && ((*in) >> bit)) {
                buffer.push_back(bit);
            }
        }

        inline void fit(std::deque<stream_t> &buffer) {
            if (buffer.size() > buffer_size) {
                shrink(buffer);
            } else if (buffer.size() == 0) {
                fill(buffer);
            }
        }

        inline stream_t last(std::deque<stream_t> &buffer) {
            if (buffer.size() == 0) {
                return EOF;
            }
            stream_t bit = buffer.front(); buffer.pop_front();
            return bit;
        }

    };

    template<typename stream_t, int64_t buffer_size>
    class Lexer {
    private:
        stream_t nextToken;
        Stream<stream_t, buffer_size> program;

    public:
        Lexer(std::istream &in) : program(&in) {
            this->nextToken = program.Read();
        }

        LexerResult parse();
        bool parseKeywords();
        bool parseOperator();
        bool parseComment();
        bool parseMarks();
        bool parseIdentifier();
        bool parseNumber();
        bool parseSpace();
        bool parseConstString();
    private:
        void reclaim(const std::basic_string<stream_t> &s);
    };
}
#endif


