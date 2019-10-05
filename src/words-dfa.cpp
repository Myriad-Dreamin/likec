

#ifndef WORDS_DFS_IMPL
#define WORDS_DFS_IMPL
#include <type_traits>
#include <stdint.h>
#include <map>

template<typename>
struct is_char: public std::false_type {
};

template<>
struct is_char<char>: public std::true_type {
};

template<>
struct is_char<wchar_t>: public std::true_type {
};

template<>
struct is_char<char16_t>: public std::true_type {
};

template<>
struct is_char<char32_t>: public std::true_type {
};

template<typename stream_t, typename accepted_type>
struct State {
    typename std::enable_if<std::is_signed<accepted_type>::value>::type type_check() {};
    using cur_type = State<stream_t, accepted_type>;
    accepted_type accepted;

    std::map<stream_t, cur_type*> nx;
    State(): accepted(-1) {}
    State(accepted_type accepted): accepted(accepted) {}
    State(const cur_type &b) : accepted(b.accepted), nx(b.nx) {}
    State(cur_type &b) : accepted(b.accepted), nx(b.nx) {}
    State(cur_type &&b) {
        accepted = b.accepted;
        nx = std::move(b.nx);
    }
    ~State() {
        for (auto &s :nx) {
            delete s.second;
        }
    }
    bool is_accepted() { return accepted != -1; }
    static bool is_accepted(accepted_type ac) { return ac != -1; }
    const cur_type* &at(const stream_t &c) { return this->nx.at(c); }
    bool count(const stream_t &c) { return this->nx.count(c); }
};


template<typename stream_t, typename accepted_type=int64_t>
class WordsDFA{
    typename std::enable_if<is_char<stream_t>::value>::type type_check() {};
    public:
    using dfa_state = State<stream_t, accepted_type>;
    private:
    dfa_state *begin;
    public:
    
    WordsDFA(const std::basic_string<stream_t> s[], size_t len) {
        this->begin = new dfa_state();
        
        for (size_t i = 0; i < len; i++) {
            build(s[i], accepted_type(i));
        }
    }
    
    WordsDFA(const std::vector<const std::basic_string<stream_t>> &v) {
        this->begin = new dfa_state();
        
        for (size_t i = 0; i < v.size(); i++) {
            build(v[i], accepted_type(i));
        }
    }

    ~WordsDFA() {
        if (this->begin) {
            delete this->begin;
        }
    }

    template<typename StreamT>
    inline accepted_type match(stream_t &current_token, StreamT &a) {
        return _match(current_token, a, begin);
    }

    static bool is_accepted(accepted_type ac) {
        return dfa_state::is_accepted(ac);
    }
    private:
    template<typename StreamT>
    accepted_type _match(stream_t &current_token, StreamT &a, const dfa_state *cur) {
        // std::cout << current_token << " " << int(current_token) << std::endl;
        if (cur->nx.count(current_token)) {
            stream_t bit = a.Read();
            std::swap(current_token, bit);
            accepted_type nx_accepted = _match(current_token, a, cur->nx.at(bit));
            if (is_accepted(nx_accepted)) {
                return nx_accepted;
            }
            a.Unread(current_token);
            current_token = bit;
            return cur->accepted;
        }
        return cur->accepted;
    } 

    void build(const std::basic_string<stream_t> &s, accepted_type accepted) {
        dfa_state *cur = begin;

        for (auto &c :s) {
            if (!cur->count(c)) {
                cur = cur->nx[c] = new dfa_state();
            } else {
                cur = cur->nx[c];
            }
        }
        cur->accepted = accepted;
    }
};


#endif