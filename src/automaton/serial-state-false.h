
#ifndef MYD_SERIAL_STATE_FALSE_H
#define MYD_SERIAL_STATE_FALSE_H
#include "definer.h"
#include <utility>
#include <vector>
#include <functional>

namespace automaton {

template<typename stream_t, typename accepted_type>
struct SerialState<stream_t, accepted_type, false> {
    typename std::enable_if<std::is_signed<accepted_type>::value>::type type_check() {};
    
    using definer = _SerialStateDefiner<stream_t, accepted_type, false>;
    using _alloc = typename definer::Allocator;
    
    using cur_type = SerialState<stream_t, accepted_type, false>;
    using predicate = std::function<bool(stream_t)>;
    using pattern = std::pair<predicate, cur_type*>;
    
    struct any_flag{};

    static const any_flag any;
    static cur_type *discard;
    accepted_type accepted;

    SerialState(accepted_type accepted);
    SerialState();
    SerialState(const cur_type &b);
    SerialState(cur_type &b);
    SerialState(cur_type &&b);

    bool is_accepted();
    static bool is_accepted(const accepted_type ac);
    
    cur_type &operator >>(const pattern &p);
    cur_type &operator>>(const std::pair<const stream_t, cur_type*> &p);
    cur_type &operator>>(const std::pair<any_flag, cur_type*> &p);
    cur_type &operator>>(const std::pair<const std::basic_string<stream_t>, cur_type*> &p);
    template<typename R, typename S>
    cur_type &operator>>(const std::pair<const std::pair<
            const std::function<
                typename std::enable_if<std::is_integral<R>::value, R>::type
                (typename std::enable_if<
                    std::is_integral<S>::value && !std::is_same<bool, S>::value,
                    S
                >::type)>,
    cur_type*>,cur_type*> &p);

    template<typename StreamT>
    inline accepted_type match(
        stream_t &current_token,
        std::basic_string<stream_t> &result,
        StreamT &a);

    static cur_type *alloc();

    static cur_type *alloc(accepted_type accepted);

    static const std::pair<stream_t, cur_type*> &&string(
        const std::basic_string<stream_t> &str, cur_type *suffer_state);
    
private:

    std::vector<pattern> patterns;

    static const std::pair<stream_t, cur_type*> &&_string(
        const std::basic_string<stream_t> &str,
        cur_type &suffer_state,
        typename std::make_signed<size_t>::type str_pos
    );

    static bool _any(stream_t c) { return c != EOF; }
};

}
#endif
