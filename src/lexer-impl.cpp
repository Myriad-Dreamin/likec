

#ifndef LEXER_IMPL
#define LEXER_IMPL

#include "const.cpp"
#include "words-dfa.cpp"
#include "lexer.h"
#include <limits>
#include <cctype>

namespace parse
{

template<typename stream_t, int64_t buffer_size>
LexerResult Lexer<stream_t, buffer_size>::parse()
{
    parseSpace();
    if (!(this->nextToken == EOF || parseKeywords() || parseOperator() || parseIdentifier()))
    {
        return LexerResult{LexerCode::NotMatch};
    }
    return LexerResult{LexerCode::OK};
}

template<typename stream_t, int64_t buffer_size>
bool Lexer<stream_t, buffer_size>::parseKeywords()
{
    static WordsDFA<stream_t> matcher(_key_words, std::extent<decltype(_key_words)>::value);
    auto accepted = matcher.match(this->nextToken, this->program);
    if (!WordsDFA<stream_t>::is_accepted(accepted)) {
        return false;
    }

    if (!(this->nextToken == EOF || parseSpace() || parseOperator())) {
        reclaim(_key_words[accepted]);
        return false;
    }
    
    return true;
}

template<typename stream_t, int64_t buffer_size>
bool Lexer<stream_t, buffer_size>::parseOperator()
{
    std::cout << this->nextToken << " " << int(this->nextToken) << std::endl;
    static WordsDFA<stream_t> matcher(_operators, std::extent<decltype(_operators)>::value);
    auto accepted = matcher.match(this->nextToken, this->program);
    if (!WordsDFA<stream_t>::is_accepted(accepted)) {
        return false;
    }

    // if (!(this->nextToken == EOF || parseSpace() || parseKeywords())) {
    //     reclaim(_operators[accepted]);
    //     return false;
    // }
    
    return true;
}

// template<typename stream_t, int64_t buffer_size>
// bool Lexer<stream_t, buffer_size>::parseMarks()
// {
//     return false;
// }

template<typename stream_t, int64_t buffer_size>
bool Lexer<stream_t, buffer_size>::parseIdentifier()
{
    if (this->nextToken == '_' || isalpha(this->nextToken)) {
        std::string t;
        t.push_back(this->nextToken);
        this->nextToken = this->program.Read();
        while(this->nextToken == '_' || isalnum(this->nextToken)) {
            t.push_back(this->nextToken);
            this->nextToken = this->program.Read();
        }
        return true;
    }
    return false;
}

// template<typename stream_t, int64_t buffer_size>
// bool Lexer<stream_t, buffer_size>::parseNumber()
// {
//     if (parseHex() || parseExponential() || parseDecimal() || parseInteger()) {
//         return true;
//     }
//         // bool parseHex();
//         // bool parseExponential();
//         // bool parseInteger();
//         // bool parseDecimal();
//     return false;
// }


template<typename stream_t, int64_t buffer_size>
bool Lexer<stream_t, buffer_size>::parseConstString()
{
    return false;
}

template<typename stream_t, int64_t buffer_size>
bool Lexer<stream_t, buffer_size>::parseComment()
{
    return false;
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
    this->program.Unread(this->nextToken);
    for (size_t i = s.size() - 1; i > 0; i--) {
        this->program.Unread(s[i]);
    }
    this->nextToken = s[0];
}

} // namespace parse

#endif
