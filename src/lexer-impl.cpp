

#ifndef LEXER_IMPL
#define LEXER_IMPL

#include "const.cpp"
#include "words-fa.h"
#include "serial-fa.cpp"
#include "lexer.h"
#include <limits>
#include <cctype>

namespace parse
{

    
template<typename stream_t, int64_t buffer_size>
LexerResult<stream_t> *Lexer<stream_t, buffer_size>::new_result()
{
    return new LexerResult<stream_t>();
}

template<typename stream_t, int64_t buffer_size>
LexerResult<stream_t> *Lexer<stream_t, buffer_size>::parse(LexerResult<stream_t> *result)
{
    this->result = result;
    this->result->register_report_handler([this](int64_t &row,int64_t &col) {
        this->program.get_row_col(row, col);
    });
    result->code = LexerCode::OK;
    while(this->nextToken != EOF) {
        parseSpace();
        this->program.record();
        if (!(this->nextToken == EOF || parseSpace() || parseComment() || parseKeywords() ||
            parseMark() || parseNumber() || parseConstString() || parseConstChar() ||
            parseOperator() || parseIdentifier()))
        {
            std::cout << "QAQ" << this->nextToken << std::endl;
            std::basic_string<stream_t> line_buf, pointer_buf;
            this->program.collect_lines(line_buf, pointer_buf);
            result->set_error(line_buf, pointer_buf);
            result->code = LexerCode::NotMatch;
            this->nextToken = this->program.Read();
            // return result;
        }
    }
    return result;
}

template<typename stream_t, int64_t buffer_size>
bool Lexer<stream_t, buffer_size>::parseKeywords()
{
    static WordsFA<stream_t> matcher(_key_words, std::extent<decltype(_key_words)>::value);
    auto accepted = matcher.match(this->nextToken, this->program);
    if (!WordsFA<stream_t>::is_accepted(accepted)) {
        return false;
    }

    if (isalnum(this->nextToken)) {
        reclaim(_key_words[accepted]);
        return false;
    }

    result->register_token(static_cast<TokenType>(
        static_cast<decltype(accepted)>(TokenType::KeywordBegin) + accepted));
    
    return true;
}

template<typename stream_t, int64_t buffer_size>
bool Lexer<stream_t, buffer_size>::parseOperator()
{
    static WordsFA<stream_t> matcher(_operators, std::extent<decltype(_operators)>::value);
    auto accepted = matcher.match(this->nextToken, this->program);
    if (!WordsFA<stream_t>::is_accepted(accepted)) {
        return false;
    }

    result->register_token(static_cast<TokenType>(
        static_cast<decltype(accepted)>(TokenType::OperatorBegin) + accepted));
    
    return true;
}

template<typename stream_t, int64_t buffer_size>
bool Lexer<stream_t, buffer_size>::parseMark()
{
    for (int16_t i = 0; i < 6; i++) {
        if (this->nextToken == _marks[i]) {
            result->register_token(static_cast<TokenType>(
                static_cast<raw_token_type>(TokenType::MarkBegin) + i));
            this->nextToken = this->program.Read();
            return true;
        }
    }
    return false;
}

template<typename stream_t, int64_t buffer_size>
bool Lexer<stream_t, buffer_size>::parseIdentifier()
{
    if (this->nextToken == '_' || isalpha(this->nextToken)) {
        std::basic_string<stream_t> buf;
        buf.push_back(this->nextToken);
        this->nextToken = this->program.Read();
        while(this->nextToken == '_' || isalnum(this->nextToken)) {
            buf.push_back(this->nextToken);
            this->nextToken = this->program.Read();
        }

        result->register_token(TokenType::IdentifierType, buf);
        return true;
    }
    return false;
}

template<typename stream_t>
bool ishex(stream_t c) {
    return isdigit(c) || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F');
};


template<typename stream_t, int64_t buffer_size>
bool Lexer<stream_t, buffer_size>::parseNumber()
{
    using fa_state = typename SerialFA<stream_t, raw_token_type>::fa_state;
    static auto build = [&]() -> fa_state* {
        fa_state    *begin                   = fa_state::alloc(),
                    *hex                     = fa_state::alloc(static_cast<raw_token_type>(TokenType::NumberHex)),
                    *integer                 = fa_state::alloc(static_cast<raw_token_type>(TokenType::NumberInteger)),
                    *decimal                 = fa_state::alloc(static_cast<raw_token_type>(TokenType::NumberDecimal)),
                    *exponential             = fa_state::alloc(static_cast<raw_token_type>(TokenType::NumberExponential)),
                    *integer_or_maybe_others = fa_state::alloc(static_cast<raw_token_type>(TokenType::NumberInteger)),
                    *maybe_hex               = fa_state::alloc(),
                    *maybe_integer           = fa_state::alloc(),
                    *maybe_decimal           = fa_state::alloc(),
                    *maybe_exp_accept_pm     = fa_state::alloc(),
                    *maybe_exp               = fa_state::alloc();
        
        (*begin)
                >> std::make_pair('0', integer_or_maybe_others)
                >> std::make_pair('.', maybe_decimal)
                >> std::make_pair(isdigit, integer);

        (*integer_or_maybe_others)
                >> std::make_pair("xX", maybe_hex)
                >> std::make_pair('.', maybe_decimal)
                >> std::make_pair(isdigit, integer);
        
        (*integer)
                >> std::make_pair(isdigit, integer)
                >> std::make_pair('.', maybe_decimal)
                >> std::make_pair('e', maybe_exp_accept_pm);
        
        (*maybe_hex)
                >> std::make_pair(ishex<stream_t>, hex);
        (*hex)  >> std::make_pair(ishex<stream_t>, hex);

        (*maybe_decimal)
                >> std::make_pair(isdigit, decimal);
        (*decimal)
                >> std::make_pair(isdigit, decimal)
                >> std::make_pair('e', maybe_exp_accept_pm);
        
        (*maybe_exp_accept_pm)
                >> std::make_pair("+-", maybe_exp)
                >> std::make_pair(isdigit, exponential);
        (*maybe_exp)
                >> std::make_pair(isdigit, exponential);
        (*exponential)
                >> std::make_pair(isdigit, exponential);
        

        return begin;
    };
    static SerialFA<stream_t, raw_token_type> matcher(build);

    std::basic_string<stream_t> buf;
    auto accepted = matcher.match(this->nextToken, buf, this->program);
    // std::cout << "?" << this->nextToken << ":" << int(this->nextToken) << " " << buf << ".." << accepted << std::endl; 
    if (!SerialFA<stream_t>::is_accepted(accepted)) {
        return false;
    }

    result->register_token(static_cast<TokenType>(accepted), buf);
    return true;
}


template<typename stream_t, int64_t buffer_size>
bool Lexer<stream_t, buffer_size>::parseConstString()
{
    using fa_state = typename SerialFA<stream_t, raw_token_type>::fa_state;
    static auto build = [&]() -> fa_state* {
        fa_state    *begin        = fa_state::alloc(),
                    *maybe_string = fa_state::alloc(),
                    *backslash = fa_state::alloc(),
                    *accept = fa_state::alloc(static_cast<raw_token_type>(TokenType::ConstStringType));
        
        (*begin)
                >> std::make_pair('"', maybe_string);
        
        (*maybe_string)
                >> std::make_pair('\n', fa_state::discard)
                >> std::make_pair(EOF, fa_state::discard)
                >> std::make_pair('\\', backslash)
                >> std::make_pair('"', accept)
                >> std::make_pair(fa_state::any, maybe_string);
                
        (*backslash)
                >> fa_state::string(line_break, maybe_string)
                >> std::make_pair(fa_state::any, maybe_string);
                // >> std::make_pair();

        return begin;
    };
    static SerialFA<stream_t, raw_token_type> matcher(build);

    std::basic_string<stream_t> buf;
    auto accepted = matcher.match(this->nextToken, buf, this->program);
    std::cout << "?" << this->nextToken << ":" << int(this->nextToken) << " " << buf << ".." << accepted << std::endl; 
    if(accepted == fa_state::discard->accepted){
        reclaim(buf);
        return false;
    }
    
    if (!SerialFA<stream_t>::is_accepted(accepted)) {
        return false;
    }

    result->register_token(static_cast<TokenType>(accepted), buf);

    return true;
}


template<typename stream_t, int64_t buffer_size>
bool Lexer<stream_t, buffer_size>::parseConstChar()
{
    using fa_state = typename SerialFA<stream_t, raw_token_type>::fa_state;
    static auto build = [&]() -> fa_state* {
        fa_state    *begin        = fa_state::alloc(),
                    *maybe_char = fa_state::alloc(),
                    *backslash = fa_state::alloc(),
                    *accept = fa_state::alloc(static_cast<raw_token_type>(TokenType::ConstCharType));
        
        (*begin)
                >> std::make_pair('\'', maybe_char);
        
        (*maybe_char)
                >> std::make_pair('\n', fa_state::discard)
                >> std::make_pair(EOF, fa_state::discard)
                >> std::make_pair('\\', backslash)
                >> std::make_pair('\'', accept)
                >> std::make_pair(fa_state::any, maybe_char);
                
        (*backslash)
                >> fa_state::string(line_break, maybe_char)
                >> std::make_pair(fa_state::any, maybe_char);
                // >> std::make_pair();

        return begin;
    };
    static SerialFA<stream_t, raw_token_type> matcher(build);

    std::basic_string<stream_t> buf;
    auto accepted = matcher.match(this->nextToken, buf, this->program);
    // std::cout << "?" << this->nextToken << ":" << int(this->nextToken) << " " << buf << ".." << accepted << std::endl; 
    if(accepted == fa_state::discard->accepted){
        reclaim(buf);
        return false;
    }
    if (!SerialFA<stream_t>::is_accepted(accepted)) {
        return false;
    }

    result->register_token(static_cast<TokenType>(accepted), buf);

    return true;
}

template<typename stream_t, int64_t buffer_size>
bool Lexer<stream_t, buffer_size>::parseComment()
{
    using fa_state = typename SerialFA<stream_t, raw_token_type>::fa_state;
    static auto build = [&]() -> fa_state* {
        fa_state   *begin        = fa_state::alloc(),
                    *maybe_comment = fa_state::alloc(),
                    *skipline = fa_state::alloc(),
                    *skiplines = fa_state::alloc(),
                    *accept = fa_state::alloc(static_cast<raw_token_type>(TokenType::CommentType));
        
        (*begin)
                >> std::make_pair('/', maybe_comment);
        
        (*maybe_comment)
                >> std::make_pair('/', skipline)
                >> std::make_pair('*', skiplines);
                
        (*skipline)
                >> std::make_pair(EOF, accept)
                >> fa_state::string(line_break, accept)
                >> std::make_pair(fa_state::any, skipline);
                
        (*skiplines)
                >> std::make_pair(EOF, accept)
                >> fa_state::string("*/", accept)
                >> std::make_pair(fa_state::any, skiplines);

        return begin;
    };
    static SerialFA<stream_t, raw_token_type> matcher(build);

    std::basic_string<stream_t> buf;
    auto accepted = matcher.match(this->nextToken, buf, this->program);
    // std::cout << "?" << this->nextToken << ":" << int(this->nextToken) << " " << buf << ".." << accepted << std::endl; 
    if (!SerialFA<stream_t>::is_accepted(accepted)) {
        return false;
    }

    result->register_token(static_cast<TokenType>(accepted), buf);

    return true;
}

template<typename stream_t, int64_t buffer_size>
bool Lexer<stream_t, buffer_size>::parseSpace()
{
    // std::cout << isspace(this->nextToken) << "////" << this->nextToken << " " << int(this->nextToken) << std::endl;
    if (isspace(this->nextToken)) {
        while(isspace(this->nextToken)) {
            // std::cout << this->nextToken << " " << int(this->nextToken) << " " << isspace(this->nextToken) << "| ";
            this->nextToken = this->program.Read();
        }
        return true;
    }
    return false;
}


template<typename stream_t, int64_t buffer_size>
void Lexer<stream_t, buffer_size>::reclaim(const std::basic_string<stream_t> &s) {
    // this->program.Unread(this->nextToken);
    this->program.Unread();
    for (size_t i = s.size() - 1; i > 0; i--) {
        this->program.Unread();
        // this->program.Unread(s[i]);
    }
    this->nextToken = s[0];
}

} // namespace parse

#endif
