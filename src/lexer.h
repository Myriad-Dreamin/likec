

#ifndef LEXER_H
#define LEXER_H

#include "const.cpp"
#include <iostream>
#include <queue>
#include <unordered_map>
#include <stdint.h>

namespace parse {

    enum LexerCode: uint16_t {
        OK,
        NotMatch,
    };

    const char *stringify(const LexerCode c) {
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

    const char *stringify(const TokenType c) {
        switch (c) {
        case KeywordAuto:
            return "KeywordAuto";
        case KeywordBreak:
            return "KeywordBreak";
        case KeywordCase:
            return "KeywordCase";
        case KeywordChar:
            return "KeywordChar";
        case KeywordConst:
            return "KeywordConst";
        case KeywordContinue:
            return "KeywordContinue";
        case KeywordDefault:
            return "KeywordDefault";
        case KeywordDo:
            return "KeywordDo";
        case KeywordDouble:
            return "KeywordDouble";
        case KeywordElse:
            return "KeywordElse";
        case KeywordEnum:
            return "KeywordEnum";
        case KeywordExtern:
            return "KeywordExtern";
        case KeywordFloat:
            return "KeywordFloat";
        case KeywordFor:
            return "KeywordFor";
        case KeywordGoto:
            return "KeywordGoto";
        case KeywordIf:
            return "KeywordIf";
        case KeywordInt:
            return "KeywordInt";
        case KeywordLong:
            return "KeywordLong";
        case KeywordRegister:
            return "KeywordRegister";
        case KeywordReturn:
            return "KeywordReturn";
        case KeywordShort:
            return "KeywordShort";
        case KeywordSigned:
            return "KeywordSigned";
        case KeywordSizeof:
            return "KeywordSizeof";
        case KeywordStruct:
            return "KeywordStruct";
        case KeywordStatic:
            return "KeywordStatic";
        case KeywordSwitch:
            return "KeywordSwitch";
        case KeywordTypedef:
            return "KeywordTypedef";
        case KeywordUnion:
            return "KeywordUnion";
        case KeywordUnsigned:
            return "KeywordUnsigned";
        case KeywordVoid:
            return "KeywordVoid";
        case KeywordVolatile:
            return "KeywordVolatile";
        case KeywordWhile:
            return "KeywordWhile";

        case OperatorAddition:
            return "OperatorAddition";
        case OperatorSubtraction:
            return "OperatorSubtraction";
        case OperatorMultiplication:
            return "OperatorMultiplication";
        case OperatorDivision:
            return "OperatorDivision";
        case OperatorModulus:
            return "OperatorModulus";
        case OperatorIncrement:
            return "OperatorIncrement";
        case OperatorDecrement:
            return "OperatorDecrement";

        case OperatorEQ:
            return "OperatorEQ";
        case OperatorNEQ:
            return "OperatorNEQ";
        case OperatorGT:
            return "OperatorGT";
        case OperatorGE:
            return "OperatorGE";
        case OperatorLT:
            return "OperatorLT";
        case OperatorLE:
            return "OperatorLE";

        case OperatorLogicalAnd:
            return "OperatorLogicalAnd";
        case OperatorLogicalInclusiveOr:
            return "OperatorLogicalInclusiveOr";
        case OperatorLogicalNegation:
            return "OperatorLogicalNegation";

        case OperatorBitwiseNot:
            return "OperatorBitwiseNot";
        case OperatorBitwiseAnd:
            return "OperatorBitwiseAnd";
        case OperatorBitwiseOr:
            return "OperatorBitwiseOr";
        case OperatorBitwiseXor:
            return "OperatorBitwiseXor";
        case OperatorBitwiseLeftShift:
            return "OperatorBitwiseLeftShift";
        case OperatorBitwiseRightShift:
            return "OperatorBitwiseRightShift";

        case OperatorSimpleAssignment:
            return "OperatorSimpleAssignment";

        case OperatorAdditionAssignment:
            return "OperatorAdditionAssignment";
        case OperatorSubtractionAssignment:
            return "OperatorSubtractionAssignment";
        case OperatorMultiplicationAssignment:
            return "OperatorMultiplicationAssignment";
        case OperatorDivisionAssignment:
            return "OperatorDivisionAssignment";
        case OperatorModuloAssignment:
            return "OperatorModuloAssignment";

        case OperatorBitwiseLeftShiftAssignment:
            return "OperatorBitwiseLeftShiftAssignment";
        case OperatorBitwiseRightShiftAssignment:
            return "OperatorBitwiseRightShiftAssignment";
        case OperatorBitwiseAndAssignment:
            return "OperatorBitwiseAndAssignment";
        case OperatorBitwiseOrAssignment:
            return "OperatorBitwiseOrAssignment";
        case OperatorBitwiseXorAssignment:
            return "OperatorBitwiseXorAssignment";

        case OperatorConditionalQuestionMark:
            return "OperatorConditionalQuestionMark";
        case OperatorConditionalSelector:
            return "OperatorConditionalSelector";
        case OperatorComma:
            return "OperatorComma";
        case OperatorMemberObject:
            return "OperatorMemberObject";
        case OperatorMemberPointer:
            return "OperatorMemberPointer";
        case IdentifierType:
            return "IdentifierType";

        case NumberHex:
            return "NumberHex";
        case NumberInteger:
            return "NumberInteger";
        case NumberDecimal:
            return "NumberDecimal";
        case NumberExponential:
            return "NumberExponential";

        case ConstStringType:
            return "ConstString";
        case CommentType:
            return "Comment";
        
        default:
            return "<error-token-type>";
        }
    }
    
    template<typename stream_t>
    struct Identifier {
        const std::basic_string<stream_t> name;
        int64_t line, column;

        Identifier(const std::basic_string<stream_t> &name) : name(name) {}


        friend std::ostream &operator <<(std::ostream &a, const Identifier<stream_t> &b) {
            a << "type(" << stringify(TokenType::IdentifierType) << "), name(" << b.name << ")";
            return a;
        }

        friend std::ostream &operator <<(std::ostream &a, const Identifier<stream_t> *b) {
            a << *b;
            return a;
        }
    };

    template<typename stream_t>
    struct Number {
        const TokenType token_type;
        const std::basic_string<stream_t> name;
        int64_t line, column;
        
        Number(TokenType token_type, const std::basic_string<stream_t> &name)
        : token_type(token_type), name(name) {}

        
        friend std::ostream &operator <<(std::ostream &a, const Number<stream_t> &b) {
            a << "type(" << stringify(b.token_type) << "), number(" << b.name << ")";
            return a;
        }

        friend std::ostream &operator <<(std::ostream &a, const Number<stream_t> *b) {
            a << *b;
            return a;
        }
    };

    template<typename stream_t>
    struct ConstString {
        const std::basic_string<stream_t> name;
        int64_t line, column;
        
        ConstString(const std::basic_string<stream_t> &name)
        : name(name) {}
        
        
        friend std::ostream &operator <<(std::ostream &a, const ConstString<stream_t> &b) {
            a << "type(" << stringify(TokenType::ConstStringType) << "), content(" << b.name << ")";
            return a;
        }

        friend std::ostream &operator <<(std::ostream &a, const ConstString<stream_t> *b) {
            a << *b;
            return a;
        }
    };

    template<typename stream_t>
    struct Comment {
        const std::basic_string<stream_t> name;
        int64_t line, column;
        
        Comment(const std::basic_string<stream_t> &name)
        : name(name) {}
        
        
        friend std::ostream &operator <<(std::ostream &a, const Comment<stream_t> &b) {
            a << "type(" << stringify(TokenType::CommentType) << "), content(" << b.name << ")";
            return a;
        }

        friend std::ostream &operator <<(std::ostream &a, const Comment<stream_t> *b) {
            a << *b;
            return a;
        }
    };

    template<typename stream_t>
    class Token {
        TokenType token_type;
        Token<stream_t>* last_token_reference;
        void* token_reference;
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
                return _key_words[token_type - TokenType::KeywordBegin];
            }
            return _error_type;
        }

        const std::string &as_operator() const {
            if (is_keyword()) {
                return _operators[token_type - TokenType::OperatorBegin];
            }
            return _error_type;
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
        

        friend std::ostream &operator <<(std::ostream &a, const Token<stream_t> &b) {
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
        
        friend std::ostream &operator <<(std::ostream &a, const Token<stream_t> *b) {
            a << *b;
            return a;
        }
    };

    template<typename stream_t>
    struct LexerResult {
        LexerCode code;
        typedef Token<stream_t> token_type;
        std::vector<token_type*> tokens;
        std::unordered_map<std::basic_string<stream_t>, token_type*> table;

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
            if (token_type::is_keyword(token_type) || token_type::is_operator(token_type)) {
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

            if (token_type::is_comment(token_type)) {
                return register_token(new Comment<stream_t>(buf));
            }

            if (token_type::is_number(token_type)) {
                return register_token(new Number<stream_t>(token_type, buf));
            }

            throw std::invalid_argument(std::string("token_type is type only, which is")
                + stringify(token_type));
        }

        ~LexerResult() {
            for(auto &token :tokens) {
                delete token;
                token = nullptr;
            }
        };

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
            tokens.push_back(token);
            return token;
        }
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


