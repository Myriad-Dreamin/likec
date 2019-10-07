
#ifndef MYD_AUTOMATON_EXPORT_H
#define MYD_AUTOMATON_EXPORT_H

#include "definer.h"
#include "map-state.h"
#include "words-fa.h"
#include "serial-state.h"
#include "serial-fa.h"

#include "default-serial-state-allocator.cpp"
#include "words-fa.cpp"

myriad_dreamin_export_namespace automaton {

template<typename stream_t=char, typename accepted_type=int64_t>
myriad_dreamin_export_class WordsFA;

template<typename stream_t=char, typename accepted_type=int64_t>
myriad_dreamin_export_class SerialFA;

}

#endif
