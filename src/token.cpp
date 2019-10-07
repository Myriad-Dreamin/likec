
#ifndef TOKEN_TYPES_CPP
#define TOKEN_TYPES_CPP

#include "definer.h"

namespace parse {

template<typename stream_t>
class LexerResult;

template<typename stream_t>
struct Identifier {
    const std::basic_string<stream_t> name;

    Identifier(const std::basic_string<stream_t> &name) : name(name) {}


    friend std::basic_ostream<stream_t> &operator <<(std::basic_ostream<stream_t> &a, const Identifier<stream_t> &b) {
        a << "type(" << stringify(TokenType::IdentifierType) << "), name(" << b.name << ")";
        return a;
    }

    friend std::basic_ostream<stream_t> &operator <<(std::basic_ostream<stream_t> &a, const Identifier<stream_t> *b) {
        a << *b;
        return a;
    }
};

template<typename stream_t>
struct Number {
    const TokenType token_type;
    const std::basic_string<stream_t> name;
    
    Number(TokenType token_type, const std::basic_string<stream_t> &name)
    : token_type(token_type), name(name) {}

    
    friend std::basic_ostream<stream_t> &operator <<(std::basic_ostream<stream_t> &a, const Number<stream_t> &b) {
        a << "type(" << stringify(b.token_type) << "), number(" << b.name << ")";
        return a;
    }

    friend std::basic_ostream<stream_t> &operator <<(std::basic_ostream<stream_t> &a, const Number<stream_t> *b) {
        a << *b;
        return a;
    }
};

template<typename stream_t>
struct ConstString {
    const std::basic_string<stream_t> name;
    
    ConstString(const std::basic_string<stream_t> &name)
    : name(name) {}
    
    
    friend std::basic_ostream<stream_t> &operator <<(std::basic_ostream<stream_t> &a, const ConstString<stream_t> &b) {
        a << "type(" << stringify(TokenType::ConstStringType) << "), content(" << b.name << ")";
        return a;
    }

    friend std::basic_ostream<stream_t> &operator <<(std::basic_ostream<stream_t> &a, const ConstString<stream_t> *b) {
        a << *b;
        return a;
    }
};

template<typename stream_t>
struct Space {
    const std::basic_string<stream_t> name;
    
    Space(const std::basic_string<stream_t> &name)
    : name(name) {}
    
    
    friend std::basic_ostream<stream_t> &operator <<(std::basic_ostream<stream_t> &a, const Space<stream_t> &b) {
        a << "type(" << stringify(TokenType::SpaceType) << "), content(" << b.name << ")";
        return a;
    }

    friend std::basic_ostream<stream_t> &operator <<(std::basic_ostream<stream_t> &a, const Space<stream_t> *b) {
        a << *b;
        return a;
    }
};

template<typename stream_t>
struct ConstChar {
    const std::basic_string<stream_t> name;
    
    ConstChar(const std::basic_string<stream_t> &name)
    : name(name) {}
    
    
    friend std::basic_ostream<stream_t> &operator <<(std::basic_ostream<stream_t> &a, const ConstChar<stream_t> &b) {
        a << "type(" << stringify(TokenType::ConstCharType) << "), content(" << b.name << ")";
        return a;
    }

    friend std::basic_ostream<stream_t> &operator <<(std::basic_ostream<stream_t> &a, const ConstChar<stream_t> *b) {
        a << *b;
        return a;
    }
};

template<typename stream_t>
struct ErrorInfo {
    const std::basic_string<stream_t> name, line, pointer;
    
    ErrorInfo(const std::basic_string<stream_t> &line, const std::basic_string<stream_t> &pointer)
    : name("error"), line(line), pointer(pointer) {}
    
    
    friend std::basic_ostream<stream_t> &operator <<(std::basic_ostream<stream_t> &a, const ErrorInfo<stream_t> &b) {
        a << "type(" << stringify(TokenType::ErrorInfoType) << ")\n";
        a << "error-line:" << b.line << "\n";
        a << "           " << b.pointer << "\n";
        return a;
    }

    friend std::basic_ostream<stream_t> &operator <<(std::basic_ostream<stream_t> &a, const ErrorInfo<stream_t> *b) {
        a << *b;
        return a;
    }
};

template<typename stream_t>
struct Comment {
    const std::basic_string<stream_t> name;
    
    Comment(const std::basic_string<stream_t> &name)
    : name(name) {}
    
    
    friend std::basic_ostream<stream_t> &operator <<(std::basic_ostream<stream_t> &a, const Comment<stream_t> &b) {
        a << "type(" << stringify(TokenType::CommentType) << "), content(" << b.name << ")";
        return a;
    }

    friend std::basic_ostream<stream_t> &operator <<(std::basic_ostream<stream_t> &a, const Comment<stream_t> *b) {
        a << *b;
        return a;
    }
};

template<typename stream_t>
class Token {
    friend class LexerResult<stream_t>;
    TokenType token_type;
    Token<stream_t>* last_token_reference;
    void* token_reference;
    int64_t line, column;
public:
    Token(TokenType token_type) {
        last_token_reference = nullptr;
        token_reference = nullptr;
        this->token_type = TokenType::ErrorType;

        if (TokenType::KeywordBegin <= token_type && token_type < TokenType::KeywordEnd) {
            this->token_type = token_type;
        }
        if (TokenType::OperatorBegin <= token_type && token_type < TokenType::OperatorEnd) {
            this->token_type = token_type;
        }
        if (TokenType::MarkBegin <= token_type && token_type < TokenType::MarkEnd) {
            this->token_type = token_type;
        }
    }

    Token(const Identifier<stream_t> *identifier) {
        last_token_reference = nullptr;
        token_reference = reinterpret_cast<void*>(const_cast<Identifier<stream_t> *>(identifier));;
        this->token_type = TokenType::IdentifierType;
    }

    Token(const ConstString<stream_t> *const_string) {
        last_token_reference = nullptr;
        token_reference = reinterpret_cast<void*>(const_cast<ConstString<stream_t> *>(const_string));;
        this->token_type = TokenType::ConstStringType;
    }

    Token(const Space<stream_t> *space) {
        last_token_reference = nullptr;
        token_reference = reinterpret_cast<void*>(const_cast<Space<stream_t> *>(space));;
        this->token_type = TokenType::SpaceType;
    }

    Token(const Comment<stream_t> *comment) {
        last_token_reference = nullptr;
        token_reference = reinterpret_cast<void*>(const_cast<Comment<stream_t> *>(comment));;
        this->token_type = TokenType::CommentType;
    }

    Token(const Number<stream_t> *num) {
        last_token_reference = nullptr;
        token_reference = reinterpret_cast<void*>(const_cast<Number<stream_t> *>(num));
        this->token_type = num->token_type;
    }

    Token(const ConstChar<stream_t> *ch) {
        last_token_reference = nullptr;
        token_reference = reinterpret_cast<void*>(const_cast<ConstChar<stream_t> *>(ch));
        this->token_type = TokenType::ConstCharType;
    }

    Token(const ErrorInfo<stream_t> *ch) {
        last_token_reference = nullptr;
        token_reference = reinterpret_cast<void*>(const_cast<ErrorInfo<stream_t> *>(ch));
        this->token_type = TokenType::ErrorInfoType;
    }

    ~Token() {
        this->token_type = TokenType::ErrorType;
        token_reference = nullptr;
    }

    void set_last(Token<stream_t> *reference) {
        if (token_type != reference->token_type) {
            throw std::invalid_argument(std::string("token_type is mismatch,this is ")
                + stringify(token_type) + ", but setting " + stringify(reference->token_type));
            return ;
        }

        last_token_reference = reference;
    }

    Token<stream_t> *last() {
        return last_token_reference;
    }

    const std::string &as_keyword() const {
        if (is_keyword()) {
            return _key_words[static_cast<raw_token_type>(token_type) - 
                static_cast<raw_token_type>(TokenType::KeywordBegin)];
        }
        return _error_type;
    }

    const std::string &as_operator() const {
        if (is_keyword()) {
            return _operators[static_cast<raw_token_type>(token_type) -
            static_cast<raw_token_type>(TokenType::OperatorBegin)];
        }
        return _error_type;
    }

    const char as_mark() const {
        if (is_mark()) {
            return _marks[static_cast<raw_token_type>(token_type) -
            static_cast<raw_token_type>(TokenType::MarkBegin)];
        }
        return _error_char_type;
    }

    const Identifier<stream_t> *as_identifier() const {
        if (is_identifier()) {
            return reinterpret_cast<const Identifier<stream_t> *>(token_reference);
        }
        return nullptr;
    }

    const ConstString<stream_t> *as_const_string() const {
        if (is_const_string()) {
            return reinterpret_cast<const struct ConstString<stream_t> *>(token_reference);
        }
        return nullptr;
    }

    const Space<stream_t> *as_space() const {
        if (is_space()) {
            return reinterpret_cast<const struct Space<stream_t> *>(token_reference);
        }
        return nullptr;
    }

    const ConstChar<stream_t> *as_const_char() const {
        if (is_const_char()) {
            return reinterpret_cast<const struct ConstChar<stream_t> *>(token_reference);
        }
        return nullptr;
    }

    const ErrorInfo<stream_t> *as_error_info() const {
        if (is_error_info()) {
            return reinterpret_cast<const struct ErrorInfo<stream_t> *>(token_reference);
        }
        return nullptr;
    }

    const Comment<stream_t> *as_comment() const {
        if (is_comment()) {
            return reinterpret_cast<const Comment<stream_t> *>(token_reference);
        }
        return nullptr;
    }
    
    const Number<stream_t> *as_number() const {
        if (is_number()) {
            return reinterpret_cast<const Number<stream_t> *>(token_reference);
        }
        return nullptr;
    }

    bool is_keyword() {
        return TokenType::KeywordBegin <= token_type && token_type < TokenType::KeywordEnd;
    }

    static bool is_keyword(TokenType token_type) {
        return TokenType::KeywordBegin <= token_type && token_type < TokenType::KeywordEnd;
    }

    bool is_operator() {
        return TokenType::OperatorBegin <= token_type && token_type < TokenType::OperatorEnd;
    }

    static bool is_operator(TokenType token_type) {
        return TokenType::OperatorBegin <= token_type && token_type < TokenType::OperatorEnd;
    }

    bool is_mark() {
        return TokenType::MarkBegin <= token_type && token_type < TokenType::MarkEnd;
    }

    static bool is_mark(TokenType token_type) {
        return TokenType::MarkBegin <= token_type && token_type < TokenType::MarkEnd;
    }
    
    bool is_identifier() const {
        return TokenType::IdentifierType == token_type;
    }

    static bool is_identifier(TokenType token_type) {
        return TokenType::IdentifierType == token_type;
    }
    
    bool is_const_string() const {
        return TokenType::ConstStringType == token_type;
    }

    static bool is_const_string(TokenType token_type) {
        return TokenType::ConstStringType == token_type;
    }
    
    bool is_space() const {
        return TokenType::SpaceType == token_type;
    }

    static bool is_space(TokenType token_type) {
        return TokenType::SpaceType == token_type;
    }

    bool is_const_char() const {
        return TokenType::ConstCharType == token_type;
    }

    static bool is_const_char(TokenType token_type) {
        return TokenType::ConstCharType == token_type;
    }
    
    bool is_error_info() const {
        return TokenType::ErrorInfoType == token_type;
    }

    static bool is_error_info(TokenType token_type) {
        return TokenType::ErrorInfoType == token_type;
    }
    
    bool is_comment() const {
        return TokenType::CommentType == token_type;
    }

    static bool is_comment(TokenType token_type) {
        return TokenType::CommentType == token_type;
    }

    bool is_number() const {
        return TokenType::ConstNumberBegin <= token_type && token_type < TokenType::ConstNumberEnd;
    }

    static bool is_number(TokenType token_type) {
        return TokenType::ConstNumberBegin <= token_type && token_type < TokenType::ConstNumberEnd;
    }
    

    friend std::basic_ostream<stream_t> &operator <<(std::basic_ostream<stream_t> &a, const Token<stream_t> &b) {
        a << "[line, column](" + std::to_string(b.line) + "," + std::to_string(b.column) + "), "; 
        a << "last token fields (" << reinterpret_cast<uint64_t>(b.last_token_reference) << "), ";
        a << "this token (" << reinterpret_cast<uint64_t>(&b) << ", ";
        if (b.is_identifier()) {
            a << b.as_identifier() << ")";
            return a;
        }

        if (b.is_const_string()) {
            a << b.as_const_string() << ")";
            return a;
        }

        if (b.is_space()) {
            a << b.as_space() << ")";
            return a;
        }

        if (b.is_const_char()) {
            a << b.as_const_char() << ")";
            return a;
        }

        if (b.is_error_info()) {
            a << b.as_error_info() << ")";
            return a;
        }

        if (b.is_comment()) {
            a << b.as_comment() << ")";
            return a;
        }

        if (b.is_number()) {
            a << b.as_number() << ")";
            return a;
        }

        a << stringify(b.token_type) << ")";
        return a;
    }
    
    friend std::basic_ostream<stream_t> &operator <<(std::basic_ostream<stream_t> &a, const Token<stream_t> *b) {
        a << *b;
        return a;
    }
};

} // namespace parse

#endif
