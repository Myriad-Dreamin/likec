

#ifndef MYD_SERIAL_STATE_FALSE_IMPL
#define MYD_SERIAL_STATE_FALSE_IMPL

namespace automaton {

template<typename stream_t, typename accepted_type>
template<typename StreamT>
inline accepted_type SerialState<stream_t, accepted_type,false>::match(
    stream_t &current_token,
    std::basic_string<stream_t> &result,
    StreamT &a) {

    // std::cout << reinterpret_cast<uint64_t>(this) << " matching" << current_token << ":" << int(current_token) << " " << this->accepted << std::endl;
    
    for (auto &p :patterns) {
        if(p.first(current_token)) {
            if (p.second == discard) {
                return this->accepted;
            }
            result.push_back(current_token);
            current_token = a.Read();
            return p.second->match(current_token, result, a);
        }
    }
    return this->accepted;
}


template<typename stream_t, typename accepted_type>
SerialState<stream_t, accepted_type,false>::SerialState(const cur_type &b) : accepted(b.accepted) {}

template<typename stream_t, typename accepted_type>
SerialState<stream_t, accepted_type,false>::SerialState(cur_type &b) : accepted(b.accepted) {}

template<typename stream_t, typename accepted_type>
SerialState<stream_t, accepted_type,false>::SerialState(cur_type &&b) {
    accepted = b.accepted;
}

template<typename stream_t, typename accepted_type>
bool SerialState<stream_t, accepted_type,false>::is_accepted() { return accepted != -1; }

template<typename stream_t, typename accepted_type>
bool SerialState<stream_t, accepted_type,false>::is_accepted(const accepted_type ac) { return ac != -1; }

template<typename stream_t, typename accepted_type>
SerialState<stream_t, accepted_type,false> &SerialState<stream_t, accepted_type,false>::operator>>(const pattern &p) {
    this->patterns.push_back(p);
    return *this;
}

template<typename stream_t, typename accepted_type>
SerialState<stream_t, accepted_type,false> &SerialState<stream_t, accepted_type,false>::operator>>(const std::pair<const stream_t, cur_type*> &p) {
    this->patterns.push_back({
        [=](stream_t c) -> bool { return p.first == c; },
        p.second,
    });
    return *this;
}

template<typename stream_t, typename accepted_type>
SerialState<stream_t, accepted_type,false> &SerialState<stream_t, accepted_type,false>::operator>>(const std::pair<any_flag, cur_type*> &p) {
    this->patterns.push_back({_any,p.second});
    return *this;
}

template<typename stream_t, typename accepted_type>
SerialState<stream_t, accepted_type,false> &SerialState<stream_t, accepted_type,false>::operator>>(const std::pair<const std::basic_string<stream_t>, cur_type*> &p) {
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

template<typename stream_t, typename accepted_type>
template<typename R, typename S>
SerialState<stream_t, accepted_type,false> &SerialState<stream_t, accepted_type,false>::operator>>(const std::pair<const std::pair<
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

template<typename stream_t, typename accepted_type>
SerialState<stream_t, accepted_type,false> *SerialState<stream_t, accepted_type,false>::alloc() {
    return _alloc::alloc(-1);}

template<typename stream_t, typename accepted_type>
SerialState<stream_t, accepted_type,false> *SerialState<stream_t, accepted_type,false>::alloc(accepted_type accepted) {
    return _alloc::alloc(accepted);}

template<typename stream_t, typename accepted_type>
SerialState<stream_t, accepted_type,false>::SerialState(accepted_type accepted): accepted(accepted) {}

template<typename stream_t, typename accepted_type>
SerialState<stream_t, accepted_type,false>::SerialState(): accepted(-1) {}

template<typename stream_t, typename accepted_type>
const std::pair<stream_t, SerialState<stream_t, accepted_type,false>*> &&
SerialState<stream_t, accepted_type,false>::string(
    const std::basic_string<stream_t> &str,
    SerialState<stream_t, accepted_type,false> *suffer_state) {
    if (str.length() <= 0) {
        throw std::invalid_argument("empty string transition is not allowed");
    }
    return std::move(_string(str, *suffer_state, str.length()-1));
}

template<typename stream_t, typename accepted_type>
const std::pair<stream_t, SerialState<stream_t, accepted_type,false>*>&&
SerialState<stream_t, accepted_type,false>::_string(
    const std::basic_string<stream_t> &str,
    SerialState<stream_t, accepted_type,false> &suffer_state,
    typename std::make_signed<size_t>::type str_pos
) {
    if (str_pos == 0) { return std::move(std::make_pair(str[str_pos], &suffer_state)); }
    return std::move(_string(str, *alloc() >> std::make_pair(str[str_pos], &suffer_state), str_pos-1));
};

template<typename stream_t, typename accepted_type>
const typename SerialState<stream_t, accepted_type,false>::any_flag
    SerialState<stream_t, accepted_type,false>::any;
    
template<typename stream_t, typename accepted_type>
typename SerialState<stream_t, accepted_type,false>::cur_type
    *SerialState<stream_t, accepted_type,false>::discard = SerialState<stream_t, accepted_type,false>::alloc(-2);

}

#endif
