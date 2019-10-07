

#ifndef DEFAULT_SERIALSTATE_ALLOCATOR_IMPL
#define DEFAULT_SERIALSTATE_ALLOCATOR_IMPL

#include "default-serial-state-allocator.h"
#include "serial-state.h"

namespace automaton {

template<typename stream_t, typename accepted_type, bool unread_flag>
_SSAlloc<stream_t, accepted_type, unread_flag>::_SSAlloc() {}

template<typename stream_t, typename accepted_type, bool unread_flag>
_SSAlloc<stream_t, accepted_type, unread_flag>::~_SSAlloc() {
    for (auto &node :allocated) {
        delete node;
        node = nullptr;
    }
    allocated.clear();
}

template<typename stream_t, typename accepted_type, bool unread_flag>
SerialState<stream_t, accepted_type, unread_flag> *
_SSAlloc<stream_t, accepted_type, unread_flag>::alloc(accepted_type accepted) {
    cur_type *node = new cur_type(accepted);
    _SSAlloc<stream_t, accepted_type, unread_flag>::allocator.allocated.push_back(node);
    return node;
}

template<typename stream_t, typename accepted_type, bool unread_flag>
void _SSAlloc<stream_t, accepted_type, unread_flag>::destroy(
    SerialState<stream_t, accepted_type, unread_flag> * &) {}

template<typename stream_t, typename accepted_type, bool unread_flag>
_SSAlloc<stream_t, accepted_type, unread_flag> _SSAlloc<stream_t, accepted_type, unread_flag>::allocator;

}

#endif