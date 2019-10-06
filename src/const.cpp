


#ifndef CCC_CONST
#define CCC_CONST
#include <string>

namespace parse {

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

enum TokenType: int16_t {
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
    Identifier,

    KeywordBegin = KeywordAuto,
    KeywordEnd = KeywordWhile + 1,
    OperatorBegin = OperatorAddition,
    OperatorEnd = OperatorMemberPointer + 1,
    ErrorType = -1,
};

}

#endif
