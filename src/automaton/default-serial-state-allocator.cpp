

#ifndef DEFAULT_SERIALSTATE_ALLOCATOR_IMPL
#define DEFAULT_SERIALSTATE_ALLOCATOR_IMPL

#include "default-serial-state-allocator.h"
#include "serial-state.h"

namespace automaton {

template<typename stream_t, typename accepted_type>
_SSAlloc<stream_t, accepted_type>::_SSAlloc() {}

template<typename stream_t, typename accepted_type>
_SSAlloc<stream_t, accepted_type>::~_SSAlloc() {
    for (auto &node :allocated) {
        delete node;
        node = nullptr;
    }
    allocated.clear();
}

template<typename stream_t, typename accepted_type>
SerialState<stream_t, accepted_type> *
_SSAlloc<stream_t, accepted_type>::alloc(accepted_type accepted) {
    cur_type *node = new cur_type(accepted);
    _SSAlloc<stream_t, accepted_type>::allocator.allocated.push_back(node);
    return node;
}

template<typename stream_t, typename accepted_type>
void _SSAlloc<stream_t, accepted_type>::destroy(
    SerialState<stream_t, accepted_type> * &) {}

template<typename stream_t, typename accepted_type>
_SSAlloc<stream_t, accepted_type> _SSAlloc<stream_t, accepted_type>::allocator;

}

#endif