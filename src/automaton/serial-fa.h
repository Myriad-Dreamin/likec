
#ifndef MYD_SERIAL_FA_H
#define MYD_SERIAL_FA_H

#include "definer.h"
#include "serial-state.h"
#include <vector>
#include <utility>
#include <functional>

namespace automaton {

template<typename stream_t, typename accepted_type>
class SerialFA
{
    typename std::enable_if<is_char<stream_t>::value>::type type_check() {};
    public:
    using fa_state = SerialState<stream_t, accepted_type>;
    SerialFA(const std::function<fa_state*()> &once) { entry = once(); }
    
    template<typename StreamT>
    inline accepted_type match(stream_t &current_token, std::basic_string<stream_t> &result, StreamT &a) {
        return entry->match(current_token, result, a);
    }
    static bool is_accepted(accepted_type accepted) {
        return fa_state::is_accepted(accepted);
    }

    private:
    fa_state *entry;
};

}

#endif
