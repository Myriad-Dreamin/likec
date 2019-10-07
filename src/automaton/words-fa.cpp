
#ifndef WORDS_DFA_IMPL
#define WORDS_DFA_IMPL

#include "words-fa.h"

namespace automaton {

template<typename stream_t, typename accepted_type>
WordsFA<stream_t, accepted_type>::WordsFA(const std::basic_string<stream_t> s[], size_t len) {
    this->begin = new dfa_state();
    
    for (size_t i = 0; i < len; i++) {
        build(s[i], accepted_type(i));
    }
}

template<typename stream_t, typename accepted_type>
WordsFA<stream_t, accepted_type>::WordsFA(const std::vector<const std::basic_string<stream_t>> &v) {
    this->begin = new dfa_state();
    
    for (size_t i = 0; i < v.size(); i++) {
        build(v[i], accepted_type(i));
    }
}

template<typename stream_t, typename accepted_type>
WordsFA<stream_t, accepted_type>::~WordsFA() {
    if (this->begin) {
        delete this->begin;
    }
}

template<typename stream_t, typename accepted_type>
bool WordsFA<stream_t, accepted_type>::is_accepted(accepted_type ac) {
    return dfa_state::is_accepted(ac);
}

template<typename stream_t, typename accepted_type>
void WordsFA<stream_t, accepted_type>::build(const std::basic_string<stream_t> &s, accepted_type accepted) {
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


template<typename stream_t, typename accepted_type>
template<typename StreamT>
inline accepted_type WordsFA<stream_t, accepted_type>::match(stream_t &current_token, StreamT &a) {
    return _match(current_token, a, begin);
}

template<typename stream_t, typename accepted_type>
template<typename StreamT>
accepted_type WordsFA<stream_t, accepted_type>::_match(stream_t &current_token, StreamT &a, const dfa_state *cur) {
    std::cout << current_token << " " << int(current_token) << std::endl;
    if (cur->nx.count(current_token)) {
        stream_t bit = a.Read();
        std::swap(current_token, bit);
        accepted_type nx_accepted = _match(current_token, a, cur->nx.at(bit));
        if (is_accepted(nx_accepted)) {
            return nx_accepted;
        }
        a.Unread();
        // a.Unread(current_token);
        current_token = bit;
        return cur->accepted;
    }
    return cur->accepted;
} 

}

#endif
