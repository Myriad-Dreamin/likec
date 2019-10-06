
#ifndef SERIAL_DFA_IMPL
#define SERIAL_DFA_IMPL
#include <stdint.h>
#include <vector>
#include <utility>
#include <functional>

namespace parse {

template<typename stream_t, typename accepted_type>
struct SerialState {
    typedef bool (*predicate)(char);
    typename std::enable_if<std::is_signed<accepted_type>::value>::type type_check() {};
    using cur_type = SerialState<stream_t, accepted_type>;
    typedef std::pair<predicate, cur_type*> pattern;
    accepted_type accepted;

    SerialState(const cur_type &b) : accepted(b.accepted) {}
    SerialState(cur_type &b) : accepted(b.accepted) {}
    SerialState(cur_type &&b) {
        accepted = b.accepted;
    }
    bool is_accepted() { return accepted != -1; }
    static bool is_accepted(const accepted_type ac) { return ac != -1; }
    cur_type *operator >>(const pattern &p) {
        this.patterns.push_back(p);
        return this;
    }

    cur_type *operator >>(const std::pair<const stream_t, cur_type*> &p) {
        this.patterns.push_back({
            [=](stream_t c) -> bool { return p.first == c; },
            p.second,
        });
        return this;
    }

    template<typename R, typename S>
    cur_type *operator >>(const std::pair<
        const std::function<
            typename std::enable_if<std::is_integral<R>::value, R>::type
            (typename std::enable_if<
                std::is_integral<S>::value && !std::is_same<bool, S>::value,
                S
            >::type)>,
        cur_type*> &p) {
        this.patterns.push_back({
            [=](stream_t c) -> bool { return p.first(c); },
            p.second,
        });
        return this;
    }

    template<typename StreamT>
    inline accepted_type match(stream_t &current_token, StreamT &a) {
        stream_t bit = a.Read();
        for (auto &p :patterns) {

            if(p.first(current_token)) {
                std::swap(current_token, bit);
                accepted_type nx_accepted = p.second->match(current_token);
                if (is_accepted(nx_accepted)) {
                    return nx_accepted;
                }
                std::swap(current_token, bit);
            }
        }
        a.Unread(bit);
        return this->accepted;
    }

    static cur_type *alloc() { return allocator.alloc();}


    struct Allocator {
        std::vector<cur_type*> allocated;
        Allocator() {};
        ~Allocator() {
            for (auto &node :allocated) {
                delete node;
                node = nullptr;
            }
            allocated.clear();
        }
        cur_type *alloc() {
            cur_type *node = new cur_type();
            allocated.push_back(node);
            return node;
        };
    };
private:
    SerialState(accepted_type accepted): accepted(accepted) {}
    SerialState(): accepted(-1) {}
    std::vector<pattern> patterns;

    static Allocator allocator;

};

template<typename stream_t, typename accepted_type>
typename SerialState<stream_t, accepted_type>::Allocator 
    SerialState<stream_t, accepted_type>::allocator;


template<typename stream_t, typename accepted_type=int64_t>
struct SerialDFA
{
    typename std::enable_if<is_char<stream_t>::value>::type type_check() {};
    public:
    using dfa_state = SerialState<stream_t, accepted_type>;
    SerialDFA(const std::function<dfa_state*()> once) { entry = once(); }
    
    template<typename StreamT>
    inline accepted_type match(stream_t &current_token, StreamT &a) {
        return entry->match(current_token, a);
    }
    private:
    dfa_state *entry;
};

}

#endif