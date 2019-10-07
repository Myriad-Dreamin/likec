
#ifndef TYPE_TRAITSSSS_H
#define TYPE_TRAITSSSS_H

#include <type_traits>

template<typename>
struct is_char: public std::false_type {
};

template<>
struct is_char<char>: public std::true_type {
};

template<>
struct is_char<wchar_t>: public std::true_type {
};

template<>
struct is_char<char16_t>: public std::true_type {
};

template<>
struct is_char<char32_t>: public std::true_type {
};

#endif
