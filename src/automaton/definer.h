

#ifndef MYD_AUTOMATON_DEFINES_H
#define MYD_AUTOMATON_DEFINES_H

#include "default-serial-state-allocator.h"

#ifndef myriad_dreamin_export_type
#define myriad_dreamin_export_type
#define myriad_dreamin_export_class class
#define myriad_dreamin_export_struct struct
#define myriad_dreamin_export_definer struct
#define myriad_dreamin_export_namespace namespace
#endif

myriad_dreamin_export_namespace automaton {

template<typename stream_t, typename accepted_type>
myriad_dreamin_export_definer _SerialStateDefiner{
    using Allocator = _SSAlloc<stream_t, accepted_type>;
};

}

#endif
