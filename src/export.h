

#ifndef MYD_LEXER_EXPORT_H
#define MYD_LEXER_EXPORT_H

#include "definer.h"
#include "lexer.h"
#include "lexer-result.h"


#include "stream.cpp"

myriad_dreamin_export_namespace parse {
    template<int64_t buffer_size>
    struct basic_lexer {
        using lexer = Lexer<char, buffer_size, Stream<char,buffer_size> >;
    };

    using kbbuf_lexer = typename basic_lexer<1<<10>::lexer;
    using mbbuf_lexer = typename basic_lexer<1<<20>::lexer;
    using gbbuf_lexer = typename basic_lexer<1<<30>::lexer;

}



#endif
