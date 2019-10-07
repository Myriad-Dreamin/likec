
#ifndef MYD_CXX_ASCII_CONST
#define MYD_CXX_ASCII_CONST
#include <string>

namespace parse {

#if (defined _WIN32)
    // static const std::string line_break = "\r\n";
    static const std::string line_break = "\n";
    const size_t line_break_length = 1;
#endif

#if (defined __linux__) || (defined linux) || (defined __linux)
    static const std::string line_break = "\n";
    const size_t line_break_length = 1;
#endif

#if ((defined __APPLE__) || (defined __MACH__))
    static const std::string line_break = "\n";
    const size_t line_break_length = 1;
#endif

const std::string _key_words[] = {
    "auto",
    "break",
    "case",
    "char",
    "const",
    "continue",
    "default",
    "do",
    "double",
    "else",
    "enum",
    "extern",
    "float",
    "for",
    "goto",
    "if",
    "int",
    "long",
    "register",
    "return",
    "short",
    "signed",
    "sizeof",
    "struct",
    "static",
    "switch",
    "typedef",
    "union",
    "unsigned",
    "void",
    "volatile",
    "while",
};

const std::string _error_type = "<error-type>";
const char _error_char_type = -2;

const std::string _operators[] = {
    "+",
    "-",
    "*",
    "/",
    "%",
    "++",
    "--",

    "==",
    "!=",
    ">",
    ">=",
    "<",
    "<=",

    "&&",
    "||",
    "!",

    "~",
    "&",
    "|",
    "^",
    "<<",
    ">>",

    "=",

    "+=",
    "-=",
    "*=",
    "/=",
    "%=",

    "<<=",
    ">>=",
    "&=",
    "|=",
    "^=",

    "?",
    ":",
    ",",
    ".",
    "->",
};

const char _marks[] = {
    '(',
    ')',
    '{',
    '}',
    '[',
    ']',
    '#',
    ';',
};

enum class TokenType: int16_t {
    KeywordAuto,
    KeywordBreak,
    KeywordCase,
    KeywordChar,
    KeywordConst,
    KeywordContinue,
    KeywordDefault,
    KeywordDo,
    KeywordDouble,
    KeywordElse,
    KeywordEnum,
    KeywordExtern,
    KeywordFloat,
    KeywordFor,
    KeywordGoto,
    KeywordIf,
    KeywordInt,
    KeywordLong,
    KeywordRegister,
    KeywordReturn,
    KeywordShort,
    KeywordSigned,
    KeywordSizeof,
    KeywordStruct,
    KeywordStatic,
    KeywordSwitch,
    KeywordTypedef,
    KeywordUnion,
    KeywordUnsigned,
    KeywordVoid,
    KeywordVolatile,
    KeywordWhile,

    OperatorAddition,
    OperatorSubtraction,
    OperatorMultiplication,
    OperatorDivision,
    OperatorModulus,
    OperatorIncrement,
    OperatorDecrement,

    OperatorEQ,
    OperatorNEQ,
    OperatorGT,
    OperatorGE,
    OperatorLT,
    OperatorLE,

    OperatorLogicalAnd,
    OperatorLogicalInclusiveOr,
    OperatorLogicalNegation,

    OperatorBitwiseNot,
    OperatorBitwiseAnd,
    OperatorBitwiseOr,
    OperatorBitwiseXor,
    OperatorBitwiseLeftShift,
    OperatorBitwiseRightShift,

    OperatorSimpleAssignment,

    OperatorAdditionAssignment,
    OperatorSubtractionAssignment,
    OperatorMultiplicationAssignment,
    OperatorDivisionAssignment,
    OperatorModuloAssignment,

    OperatorBitwiseLeftShiftAssignment,
    OperatorBitwiseRightShiftAssignment,
    OperatorBitwiseAndAssignment,
    OperatorBitwiseOrAssignment,
    OperatorBitwiseXorAssignment,

    OperatorConditionalQuestionMark,
    OperatorConditionalSelector,
    OperatorComma,
    OperatorMemberObject,
    OperatorMemberPointer,
    
    MarkLPAREN,
    MarkRPAREN,
    MarkLBRACE,
    MarkRBRACE,
    MarkLSQUARE,
    MarkRSQUARE,
    MarkSharp,
    MarkSemicolon,

    NumberHex,
    NumberInteger,
    NumberDecimal,
    NumberExponential,

    IdentifierType,
    ConstStringType,
    CommentType,
    ConstCharType,
    ErrorInfoType,

    KeywordBegin = KeywordAuto,
    KeywordEnd = KeywordWhile + 1,
    OperatorBegin = OperatorAddition,
    OperatorEnd = OperatorMemberPointer + 1,
    ConstNumberBegin = NumberHex,
    ConstNumberEnd = NumberExponential + 1,
    MarkBegin = MarkLPAREN,
    MarkEnd = MarkSemicolon + 1,
    ErrorType = -1,
};

typedef std::underlying_type<TokenType>::type raw_token_type;

const raw_token_type KeywordRange = 
    static_cast<raw_token_type>(TokenType::KeywordEnd) - static_cast<raw_token_type>(TokenType::KeywordBegin);
const raw_token_type OperatorRange = 
    static_cast<raw_token_type>(TokenType::OperatorEnd) - static_cast<raw_token_type>(TokenType::OperatorBegin);
const raw_token_type ConstNumberRange = 
    static_cast<raw_token_type>(TokenType::ConstNumberEnd) - static_cast<raw_token_type>(TokenType::ConstNumberBegin);
const raw_token_type MarkRange = 
    static_cast<raw_token_type>(TokenType::MarkEnd) - static_cast<raw_token_type>(TokenType::MarkBegin);


enum class LexerCode: uint16_t {
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
    case TokenType::KeywordAuto:
        return "KeywordAuto";
    case TokenType::KeywordBreak:
        return "KeywordBreak";
    case TokenType::KeywordCase:
        return "KeywordCase";
    case TokenType::KeywordChar:
        return "KeywordChar";
    case TokenType::KeywordConst:
        return "KeywordConst";
    case TokenType::KeywordContinue:
        return "KeywordContinue";
    case TokenType::KeywordDefault:
        return "KeywordDefault";
    case TokenType::KeywordDo:
        return "KeywordDo";
    case TokenType::KeywordDouble:
        return "KeywordDouble";
    case TokenType::KeywordElse:
        return "KeywordElse";
    case TokenType::KeywordEnum:
        return "KeywordEnum";
    case TokenType::KeywordExtern:
        return "KeywordExtern";
    case TokenType::KeywordFloat:
        return "KeywordFloat";
    case TokenType::KeywordFor:
        return "KeywordFor";
    case TokenType::KeywordGoto:
        return "KeywordGoto";
    case TokenType::KeywordIf:
        return "KeywordIf";
    case TokenType::KeywordInt:
        return "KeywordInt";
    case TokenType::KeywordLong:
        return "KeywordLong";
    case TokenType::KeywordRegister:
        return "KeywordRegister";
    case TokenType::KeywordReturn:
        return "KeywordReturn";
    case TokenType::KeywordShort:
        return "KeywordShort";
    case TokenType::KeywordSigned:
        return "KeywordSigned";
    case TokenType::KeywordSizeof:
        return "KeywordSizeof";
    case TokenType::KeywordStruct:
        return "KeywordStruct";
    case TokenType::KeywordStatic:
        return "KeywordStatic";
    case TokenType::KeywordSwitch:
        return "KeywordSwitch";
    case TokenType::KeywordTypedef:
        return "KeywordTypedef";
    case TokenType::KeywordUnion:
        return "KeywordUnion";
    case TokenType::KeywordUnsigned:
        return "KeywordUnsigned";
    case TokenType::KeywordVoid:
        return "KeywordVoid";
    case TokenType::KeywordVolatile:
        return "KeywordVolatile";
    case TokenType::KeywordWhile:
        return "KeywordWhile";

    case TokenType::OperatorAddition:
        return "OperatorAddition";
    case TokenType::OperatorSubtraction:
        return "OperatorSubtraction";
    case TokenType::OperatorMultiplication:
        return "OperatorMultiplication";
    case TokenType::OperatorDivision:
        return "OperatorDivision";
    case TokenType::OperatorModulus:
        return "OperatorModulus";
    case TokenType::OperatorIncrement:
        return "OperatorIncrement";
    case TokenType::OperatorDecrement:
        return "OperatorDecrement";

    case TokenType::OperatorEQ:
        return "OperatorEQ";
    case TokenType::OperatorNEQ:
        return "OperatorNEQ";
    case TokenType::OperatorGT:
        return "OperatorGT";
    case TokenType::OperatorGE:
        return "OperatorGE";
    case TokenType::OperatorLT:
        return "OperatorLT";
    case TokenType::OperatorLE:
        return "OperatorLE";

    case TokenType::OperatorLogicalAnd:
        return "OperatorLogicalAnd";
    case TokenType::OperatorLogicalInclusiveOr:
        return "OperatorLogicalInclusiveOr";
    case TokenType::OperatorLogicalNegation:
        return "OperatorLogicalNegation";

    case TokenType::OperatorBitwiseNot:
        return "OperatorBitwiseNot";
    case TokenType::OperatorBitwiseAnd:
        return "OperatorBitwiseAnd";
    case TokenType::OperatorBitwiseOr:
        return "OperatorBitwiseOr";
    case TokenType::OperatorBitwiseXor:
        return "OperatorBitwiseXor";
    case TokenType::OperatorBitwiseLeftShift:
        return "OperatorBitwiseLeftShift";
    case TokenType::OperatorBitwiseRightShift:
        return "OperatorBitwiseRightShift";

    case TokenType::OperatorSimpleAssignment:
        return "OperatorSimpleAssignment";

    case TokenType::OperatorAdditionAssignment:
        return "OperatorAdditionAssignment";
    case TokenType::OperatorSubtractionAssignment:
        return "OperatorSubtractionAssignment";
    case TokenType::OperatorMultiplicationAssignment:
        return "OperatorMultiplicationAssignment";
    case TokenType::OperatorDivisionAssignment:
        return "OperatorDivisionAssignment";
    case TokenType::OperatorModuloAssignment:
        return "OperatorModuloAssignment";

    case TokenType::OperatorBitwiseLeftShiftAssignment:
        return "OperatorBitwiseLeftShiftAssignment";
    case TokenType::OperatorBitwiseRightShiftAssignment:
        return "OperatorBitwiseRightShiftAssignment";
    case TokenType::OperatorBitwiseAndAssignment:
        return "OperatorBitwiseAndAssignment";
    case TokenType::OperatorBitwiseOrAssignment:
        return "OperatorBitwiseOrAssignment";
    case TokenType::OperatorBitwiseXorAssignment:
        return "OperatorBitwiseXorAssignment";

    case TokenType::OperatorConditionalQuestionMark:
        return "OperatorConditionalQuestionMark";
    case TokenType::OperatorConditionalSelector:
        return "OperatorConditionalSelector";
    case TokenType::OperatorComma:
        return "OperatorComma";
    case TokenType::OperatorMemberObject:
        return "OperatorMemberObject";
    case TokenType::OperatorMemberPointer:
        return "OperatorMemberPointer";
        
    case TokenType::MarkLPAREN:
        return "MarkLPAREN";
    case TokenType::MarkRPAREN:
        return "MarkRPAREN";
    case TokenType::MarkLBRACE:
        return "MarkLBRACE";
    case TokenType::MarkRBRACE:
        return "MarkRBRACE";
    case TokenType::MarkLSQUARE:
        return "MarkLSQUARE";
    case TokenType::MarkRSQUARE:
        return "MarkRSQUARE";
    case TokenType::MarkSharp:
        return "MarkSharp";
    case TokenType::MarkSemicolon:
        return "MarkSemicolon";
        
    case TokenType::NumberHex:
        return "NumberHex";
    case TokenType::NumberInteger:
        return "NumberInteger";
    case TokenType::NumberDecimal:
        return "NumberDecimal";
    case TokenType::NumberExponential:
        return "NumberExponential";

    case TokenType::IdentifierType:
        return "IdentifierType";
    case TokenType::ConstStringType:
        return "ConstString";
    case TokenType::CommentType:
        return "Comment";
    case TokenType::ErrorInfoType:
        return "Error";
    case TokenType::ConstCharType:
        return "ConstChar";
    
    default:
        return "<error-token-type>";
    }
}

}

#endif
