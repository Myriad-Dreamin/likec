
#ifndef MYD_SERIAL_STATE_H
#define MYD_SERIAL_STATE_H
#include "definer.h"

namespace automaton {

template<typename stream_t, typename accepted_type>
struct SerialState {
    typename std::enable_if<std::is_signed<accepted_type>::value>::type type_check() {};
    
    using definer = _SerialStateDefiner<stream_t, accepted_type>;
    using _alloc = typename definer::Allocator;
    
    using cur_type = SerialState<stream_t, accepted_type>;
    using predicate = std::function<bool(stream_t)>;
    using pattern = std::pair<predicate, cur_type*>;
    
    struct any_flag{};

    static const any_flag any;
    static cur_type *discard;
    accepted_type accepted;

    SerialState(accepted_type accepted);
    SerialState();
    SerialState(const cur_type &b) : accepted(b.accepted) {}
    SerialState(cur_type &b) : accepted(b.accepted) {}
    SerialState(cur_type &&b) {
        accepted = b.accepted;
    }
    bool is_accepted() { return accepted != -1; }
    static bool is_accepted(const accepted_type ac) { return ac != -1; }
    cur_type &operator >>(const pattern &p) {
        this->patterns.push_back(p);
        return *this;
    }

    cur_type &operator >>(const std::pair<const stream_t, cur_type*> &p) {
        this->patterns.push_back({
            [=](stream_t c) -> bool { return p.first == c; },
            p.second,
        });
        return *this;
    }

    cur_type &operator >>(const std::pair<any_flag, cur_type*> &p) {
        this->patterns.push_back({_any,p.second});
        return *this;
    }

    cur_type &operator >>(const std::pair<const std::basic_string<stream_t>, cur_type*> &p) {
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

    template<typename R, typename S>
    cur_type &operator >>(const std::pair<const std::pair<
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

    template<typename StreamT>
    inline accepted_type match(
        stream_t &current_token,
        std::basic_string<stream_t> &result,
        StreamT &a) {

        // std::cout << reinterpret_cast<uint64_t>(this) << " matching" << current_token << ":" << int(current_token) << " " << this->accepted << std::endl;
        if (this == discard) {
            return this->accepted;
        }
        
        stream_t bit = a.Read();
        for (auto &p :patterns) {
            if(p.first(current_token)) {
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

    static cur_type *alloc() {
        return _alloc::alloc(-1);}

    static cur_type *alloc(accepted_type accepted) {
        return _alloc::alloc(accepted);}

    static const std::pair<stream_t, cur_type*> &&string(const std::basic_string<stream_t> &str, cur_type *suffer_state) {
        if (str.length() <= 0) {
            throw std::invalid_argument("empty string transition is not allowed");
        }
        return std::move(_string(str, *suffer_state, str.length()-1));
    }
private:

    std::vector<pattern> patterns;

    static const std::pair<stream_t, cur_type*> &&_string(
        const std::basic_string<stream_t> &str,
        cur_type &suffer_state,
        typename std::make_signed<size_t>::type str_pos
    );

    static bool _any(stream_t c) { return c != EOF; }
};

template<typename stream_t, typename accepted_type>
SerialState<stream_t, accepted_type>::SerialState(accepted_type accepted): accepted(accepted) {}

template<typename stream_t, typename accepted_type>
SerialState<stream_t, accepted_type>::SerialState(): accepted(-1) {}

template<typename stream_t, typename accepted_type>
const std::pair<stream_t, SerialState<stream_t, accepted_type>*>&&
SerialState<stream_t, accepted_type>::_string(
    const std::basic_string<stream_t> &str,
    SerialState<stream_t, accepted_type> &suffer_state,
    typename std::make_signed<size_t>::type str_pos
) {
    if (str_pos == 0) { return std::move(std::make_pair(str[str_pos], &suffer_state)); }
    return std::move(_string(str, *alloc() >> std::make_pair(str[str_pos], &suffer_state), str_pos-1));
};

template<typename stream_t, typename accepted_type>
const typename SerialState<stream_t, accepted_type>::any_flag
    SerialState<stream_t, accepted_type>::any;
    
template<typename stream_t, typename accepted_type>
typename SerialState<stream_t, accepted_type>::cur_type
    *SerialState<stream_t, accepted_type>::discard = SerialState<stream_t, accepted_type>::alloc(-2);

}

#endif
