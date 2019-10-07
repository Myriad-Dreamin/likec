

#ifndef MYD_LEXER_EXPORT_H
#define MYD_LEXER_EXPORT_H

#include "definer.h"
#include "lexer.h"
#include "lexer-result.h"


#include "stream.cpp"

myriad_dreamin_export_namespace parse {
    template<int64_t buffer_size>
    using basic_lexer = Lexer<char, buffer_size, Stream<char,buffer_size>, true>;

    using kbbuf_lexer = basic_lexer<1<<10>;
    using mbbuf_lexer = basic_lexer<1<<20>;
    using gbbuf_lexer = basic_lexer<1<<30>;
}

#endif
