

#ifndef LEXER_IMPL
#define LEXER_IMPL

#include "lexer.h"
#include "automaton/export.h"
#include <cctype>

namespace parse
{

template<typename stream_t, int64_t buffer_size, class Stream, bool unread_flag>
LexerResult<stream_t> *Lexer<stream_t, buffer_size, Stream, unread_flag>::new_result()
{
    return new LexerResult<stream_t>();
}

template<typename stream_t, int64_t buffer_size, class Stream, bool unread_flag>
Lexer<stream_t, buffer_size, Stream, unread_flag>::Lexer(istream &in) : program(&in) {
    this->nextToken = program.Read();
}

template<typename stream_t, int64_t buffer_size, class Stream, bool unread_flag>
LexerResult<stream_t> *Lexer<stream_t, buffer_size, Stream, unread_flag>::parse(LexerResult<stream_t> *result)
{
    this->result = result;
    result->register_report_handler([this](int64_t &row,int64_t &col) {
        program.get_row_col(row, col);
    });
    result->code = LexerCode::OK;
    bool flag;
    while(nextToken != EOF) {
        parseSpace();
        program.record();
        switch (nextToken)
        {
        case EOF:
            goto return_result;
        case '/':
            flag = parseComment() || parseOperator();
            break;
        case '"':
            flag = parseConstString();
            break;
        case '\'':
            flag = parseConstChar();
            break;
        case '.':
            flag = parseNumber() || parseOperator();
            break;
        default:
            if (isdigit(nextToken)) {
                flag = parseNumber();
            } else if (nextToken == '_' || isalpha(nextToken)) {
                flag = parseKeywords() || parseIdentifier();
            } else {
                flag = parseMark() || parseOperator();
            }
            break;
        }

        if (!flag){
            // std::cout << "QAQ" << nextToken << std::endl;
            std::basic_string<stream_t> line_buf, pointer_buf;
            program.collect_lines(line_buf, pointer_buf);
            result->set_error(line_buf, pointer_buf);
            result->code = LexerCode::NotMatch;
            nextToken = program.Read();
            // return result;
        }
    }

    return_result:
    result->set_lines(program.lines());
    result->set_offset(program.offset());
    return result;
}

template<typename stream_t, int64_t buffer_size, class Stream, bool unread_flag>
bool Lexer<stream_t, buffer_size, Stream, unread_flag>::parseKeywords()
{
    using namespace automaton;
    static WordsFA<stream_t, raw_token_type> matcher(_key_words, std::extent<decltype(_key_words)>::value);
    auto accepted = matcher.match(this->nextToken, this->program);
    if (!WordsFA<stream_t, raw_token_type>::is_accepted(accepted)) {
        return false;
    }

    if (isalnum(this->nextToken)) {
        set_buf(_key_words[accepted]);
        return false;
    }

    result->register_token(static_cast<TokenType>(
        static_cast<decltype(accepted)>(TokenType::KeywordBegin) + accepted));
    
    return true;
}

template<typename stream_t, int64_t buffer_size, class Stream, bool unread_flag>
bool Lexer<stream_t, buffer_size, Stream, unread_flag>::parseOperator()
{
    using namespace automaton;
    static WordsFA<stream_t, raw_token_type> matcher(_operators, std::extent<decltype(_operators)>::value);
    auto accepted = matcher.match(this->nextToken, this->program);
    if (!WordsFA<stream_t, raw_token_type>::is_accepted(accepted)) {
        return false;
    }

    result->register_token(static_cast<TokenType>(
        static_cast<decltype(accepted)>(TokenType::OperatorBegin) + accepted));
    
    return true;
}

template<typename stream_t, int64_t buffer_size, class Stream, bool unread_flag>
bool Lexer<stream_t, buffer_size, Stream, unread_flag>::parseMark()
{
    for (int16_t i = 0; i < MarkRange; i++) {
        if (this->nextToken == _marks[i]) {
            result->register_token(static_cast<TokenType>(
                static_cast<raw_token_type>(TokenType::MarkBegin) + i));
            this->nextToken = this->program.Read();
            return true;
        }
    }
    return false;
}

template<typename stream_t, int64_t buffer_size, class Stream, bool unread_flag>
bool Lexer<stream_t, buffer_size, Stream, unread_flag>::parseIdentifier()
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


template<typename stream_t, int64_t buffer_size, class Stream, bool unread_flag>
bool Lexer<stream_t, buffer_size, Stream, unread_flag>::parseNumber()
{
    using namespace automaton;
    using fa_state = typename SerialFA<stream_t, raw_token_type, unread_flag>::fa_state;
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
    static SerialFA<stream_t, raw_token_type, unread_flag> matcher(build);

    std::basic_string<stream_t> buf;
    auto accepted = matcher.match(this->nextToken, buf, this->program);
    // std::cout << "?" << this->nextToken << ":" << int(this->nextToken) << " " << buf << ".." << accepted << std::endl; 
    if (!SerialFA<stream_t>::is_accepted(accepted)) {
        return false;
    }

    result->register_token(static_cast<TokenType>(accepted), buf);
    return true;
}


template<typename stream_t, int64_t buffer_size, class Stream, bool unread_flag>
bool Lexer<stream_t, buffer_size, Stream, unread_flag>::parseConstString()
{
    using namespace automaton;
    using fa_state = typename SerialFA<stream_t, raw_token_type, false>::fa_state;
    static auto build = [&]() -> fa_state* {
        fa_state    *begin        = fa_state::alloc(),
                    *maybe_string = fa_state::alloc(),
                    *backslash = fa_state::alloc(),
                    *accept = fa_state::alloc(static_cast<raw_token_type>(TokenType::ConstStringType));
        
        (*begin)
                >> std::make_pair('"', maybe_string);
        
        (*maybe_string)
                >> std::make_pair('\n', fa_state::discard)
                >> std::make_pair('\\', backslash)
                >> std::make_pair('"', accept)
                >> std::make_pair(fa_state::any, maybe_string);
                
        (*backslash)
                >> fa_state::string(line_break, maybe_string)
                >> std::make_pair(fa_state::any, maybe_string);
                // >> std::make_pair();

        return begin;
    };
    static SerialFA<stream_t, raw_token_type, false> matcher(build);

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


template<typename stream_t, int64_t buffer_size, class Stream, bool unread_flag>
bool Lexer<stream_t, buffer_size, Stream, unread_flag>::parseConstChar()
{
    using namespace automaton;
    using fa_state = typename SerialFA<stream_t, raw_token_type, unread_flag>::fa_state;
    static auto build = [&]() -> fa_state* {
        fa_state    *begin        = fa_state::alloc(),
                    *maybe_char = fa_state::alloc(),
                    *backslash = fa_state::alloc(),
                    *accept = fa_state::alloc(static_cast<raw_token_type>(TokenType::ConstCharType));
        
        (*begin)
                >> std::make_pair('\'', maybe_char);
        
        (*maybe_char)
                >> std::make_pair('\n', fa_state::discard)
                >> std::make_pair('\\', backslash)
                >> std::make_pair('\'', accept)
                >> std::make_pair(fa_state::any, maybe_char);
                
        (*backslash)
                >> fa_state::string(line_break, maybe_char)
                >> std::make_pair(fa_state::any, maybe_char);
                // >> std::make_pair();

        return begin;
    };
    static SerialFA<stream_t, raw_token_type, unread_flag> matcher(build);

    std::basic_string<stream_t> buf;
    auto accepted = matcher.match(this->nextToken, buf, this->program);
    // std::cout << "?" << this->nextToken << ":" << int(this->nextToken) << " " << buf << ".." << accepted << std::endl; 
    if(accepted == fa_state::discard->accepted){
        reclaim(buf);
        return false;
    }
    if (!SerialFA<stream_t, raw_token_type, unread_flag>::is_accepted(accepted)) {
        return false;
    }

    result->register_token(static_cast<TokenType>(accepted), buf);

    return true;
}

template<typename stream_t, int64_t buffer_size, class Stream, bool unread_flag>
bool Lexer<stream_t, buffer_size, Stream, unread_flag>::parseComment()
{
    using namespace automaton;
    using fa_state = typename SerialFA<stream_t, raw_token_type, unread_flag>::fa_state;
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
                >> fa_state::string(line_break, accept)
                >> std::make_pair(fa_state::any, skipline);
                
        (*skiplines)
                >> fa_state::string("*/", accept)
                >> std::make_pair(fa_state::any, skiplines);

        return begin;
    };
    static SerialFA<stream_t, raw_token_type, unread_flag> matcher(build);

    std::basic_string<stream_t> buf;
    auto accepted = matcher.match(this->nextToken, buf, this->program);
    // std::cout << "?" << this->nextToken << ":" << int(this->nextToken) << " " << buf << ".." << accepted << std::endl; 
    if (!SerialFA<stream_t, raw_token_type, unread_flag>::is_accepted(accepted)) {
        return false;
    }

    result->register_token(static_cast<TokenType>(accepted), buf);

    return true;
}

template<typename stream_t, int64_t buffer_size, class Stream, bool unread_flag>
bool Lexer<stream_t, buffer_size, Stream, unread_flag>::parseSpace()
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


template<typename stream_t, int64_t buffer_size, class Stream, bool unread_flag>
void Lexer<stream_t, buffer_size, Stream, unread_flag>::reclaim(const std::basic_string<stream_t> &s) {
    // this->program.Unread(this->nextToken);
    this->program.Unread();
    for (size_t i = s.size() - 1; i > 0; i--) {
        this->program.Unread();
        // this->program.Unread(s[i]);
    }
    this->nextToken = s[0];
}

template<typename stream_t, int64_t buffer_size, class Stream, bool unread_flag>
void Lexer<stream_t, buffer_size, Stream, unread_flag>::set_buf(const std::basic_string<stream_t> &s) {
    // this->program.Unread(this->nextToken);
    buf = s;
}

} // namespace parse

#endif
