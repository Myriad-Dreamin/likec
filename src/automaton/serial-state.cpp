

#ifndef MYD_SERIAL_STATE_IMPL
#define MYD_SERIAL_STATE_IMPL

#include "serial-state.h"

namespace automaton {

template<typename stream_t, typename accepted_type, bool unread_flag>
template<typename StreamT>
inline accepted_type SerialState<stream_t, accepted_type,unread_flag>::match(
    stream_t &current_token,
    std::basic_string<stream_t> &result,
    StreamT &a) {

    // std::cout << reinterpret_cast<uint64_t>(this) << " matching" << current_token << ":" << int(current_token) << " " << this->accepted << std::endl;
    if (this == discard) {
        return discard->accepted;
    }
    
    stream_t bit = a.Read();
    for (auto &p :patterns) {
        if(p.first(current_token)) {
            if (p.second == discard) {
                // std::cout << "discard" << std::endl;
                return discard->accepted;
            }
            result.push_back(current_token);
            current_token = bit;
            accepted_type nx_accepted = p.second->match(current_token, result, a);
            if (is_accepted(nx_accepted)) {
                return nx_accepted;
            }
            current_token = result.back();
            result.pop_back();
        }
    }
    // std::cout << reinterpret_cast<uint64_t>(this) << " matched" << int(current_token) << " " << this->accepted << std::endl;
    // a.Unread(bit);
    a.Unread();
    return this->accepted;
}


template<typename stream_t, typename accepted_type, bool unread_flag>
SerialState<stream_t, accepted_type,unread_flag>::SerialState(const cur_type &b) : accepted(b.accepted) {}

template<typename stream_t, typename accepted_type, bool unread_flag>
SerialState<stream_t, accepted_type,unread_flag>::SerialState(cur_type &b) : accepted(b.accepted) {}

template<typename stream_t, typename accepted_type, bool unread_flag>
SerialState<stream_t, accepted_type,unread_flag>::SerialState(cur_type &&b) {
    accepted = b.accepted;
}

template<typename stream_t, typename accepted_type, bool unread_flag>
bool SerialState<stream_t, accepted_type,unread_flag>::is_accepted() { return accepted != -1; }

template<typename stream_t, typename accepted_type, bool unread_flag>
bool SerialState<stream_t, accepted_type,unread_flag>::is_accepted(const accepted_type ac) { return ac != -1; }

template<typename stream_t, typename accepted_type, bool unread_flag>
SerialState<stream_t, accepted_type,unread_flag> &SerialState<stream_t, accepted_type,unread_flag>::operator>>(const pattern &p) {
    this->patterns.push_back(p);
    return *this;
}

template<typename stream_t, typename accepted_type, bool unread_flag>
SerialState<stream_t, accepted_type,unread_flag> &SerialState<stream_t, accepted_type,unread_flag>::operator>>(const std::pair<const stream_t, cur_type*> &p) {
    this->patterns.push_back({
        [=](stream_t c) -> bool { return p.first == c; },
        p.second,
    });
    return *this;
}

template<typename stream_t, typename accepted_type, bool unread_flag>
SerialState<stream_t, accepted_type,unread_flag> &SerialState<stream_t, accepted_type,unread_flag>::operator>>(const std::pair<any_flag, cur_type*> &p) {
    this->patterns.push_back({_any,p.second});
    return *this;
}

template<typename stream_t, typename accepted_type, bool unread_flag>
SerialState<stream_t, accepted_type,unread_flag> &SerialState<stream_t, accepted_type,unread_flag>::operator>>(const std::pair<const std::basic_string<stream_t>, cur_type*> &p) {
    this->patterns.push_back({
        [=](stream_t c) -> bool {
            for (auto a: p.first) {
                if (a == c) {
                    return true;
                }
            }
            return false;
        },
        p.second,
    });
    return *this;
}

template<typename stream_t, typename accepted_type, bool unread_flag>
template<typename R, typename S>
SerialState<stream_t, accepted_type,unread_flag> &SerialState<stream_t, accepted_type,unread_flag>::operator>>(const std::pair<const std::pair<
        const std::function<
            typename std::enable_if<std::is_integral<R>::value, R>::type
            (typename std::enable_if<
                std::is_integral<S>::value && !std::is_same<bool, S>::value,
                S
            >::type)>,
cur_type*>,cur_type*> &p) {
    this->patterns.push_back(p);
    return *this;
}

template<typename stream_t, typename accepted_type, bool unread_flag>
SerialState<stream_t, accepted_type,unread_flag> *SerialState<stream_t, accepted_type,unread_flag>::alloc() {
    return _alloc::alloc(-1);}

template<typename stream_t, typename accepted_type, bool unread_flag>
SerialState<stream_t, accepted_type,unread_flag> *SerialState<stream_t, accepted_type,unread_flag>::alloc(accepted_type accepted) {
    return _alloc::alloc(accepted);}

template<typename stream_t, typename accepted_type, bool unread_flag>
SerialState<stream_t, accepted_type,unread_flag>::SerialState(accepted_type accepted): accepted(accepted) {}

template<typename stream_t, typename accepted_type, bool unread_flag>
SerialState<stream_t, accepted_type,unread_flag>::SerialState(): accepted(-1) {}

template<typename stream_t, typename accepted_type, bool unread_flag>
const std::pair<stream_t, SerialState<stream_t, accepted_type,unread_flag>*> &&
SerialState<stream_t, accepted_type,unread_flag>::string(
    const std::basic_string<stream_t> &str,
    SerialState<stream_t, accepted_type,unread_flag> *suffer_state) {
    if (str.length() <= 0) {
        throw std::invalid_argument("empty string transition is not allowed");
    }
    return std::move(_string(str, *suffer_state, str.length()-1));
}

template<typename stream_t, typename accepted_type, bool unread_flag>
const std::pair<stream_t, SerialState<stream_t, accepted_type,unread_flag>*>&&
SerialState<stream_t, accepted_type,unread_flag>::_string(
    const std::basic_string<stream_t> &str,
    SerialState<stream_t, accepted_type,unread_flag> &suffer_state,
    typename std::make_signed<size_t>::type str_pos
) {
    if (str_pos == 0) { return std::move(std::make_pair(str[str_pos], &suffer_state)); }
    return std::move(_string(str, *alloc() >> std::make_pair(str[str_pos], &suffer_state), str_pos-1));
};

template<typename stream_t, typename accepted_type, bool unread_flag>
const typename SerialState<stream_t, accepted_type,unread_flag>::any_flag
    SerialState<stream_t, accepted_type,unread_flag>::any;
    
template<typename stream_t, typename accepted_type, bool unread_flag>
typename SerialState<stream_t, accepted_type,unread_flag>::cur_type
    *SerialState<stream_t, accepted_type,unread_flag>::discard = SerialState<stream_t, accepted_type,unread_flag>::alloc(-2);

}

#include "serial-state-false.cpp"

#endif
