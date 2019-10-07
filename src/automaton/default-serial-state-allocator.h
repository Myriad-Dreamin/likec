
#ifndef MYD_DEFAULT_SERIALSTATE_ALLOCATOR_H
#define MYD_DEFAULT_SERIALSTATE_ALLOCATOR_H
#include <vector>


namespace automaton {
template<typename stream_t, typename accepted_type, bool unread_flag>
struct SerialState;

template<typename stream_t, typename accepted_type, bool unread_flag>
struct _SSAlloc {
    using cur_type = SerialState<stream_t, accepted_type, unread_flag>;

    static _SSAlloc allocator;
    static cur_type *alloc(accepted_type accepted);
    static void destroy(cur_type* &state);
private:
    std::vector<cur_type*> allocated;
    _SSAlloc();
    ~_SSAlloc(); 
};

}

#endif
