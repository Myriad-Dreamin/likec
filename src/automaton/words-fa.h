
#ifndef WORDS_DFA_H
#define WORDS_DFA_H

#include "definer.h"
#include "map-state.h"
#include "type-traits.h"

namespace automaton {

template<typename stream_t, typename accepted_type>
class WordsFA{
    typename std::enable_if<is_char<stream_t>::value>::type type_check() {};
    public:
    using dfa_state = MapState<stream_t, accepted_type>;
    private:
    dfa_state *begin;
    public:
    
    WordsFA(const std::basic_string<stream_t> s[], size_t len);
    
    WordsFA(const std::vector<const std::basic_string<stream_t>> &v);

    ~WordsFA();

    template<typename StreamT>
    inline accepted_type match(stream_t &current_token, StreamT &a);
    static bool is_accepted(accepted_type ac);

    private:
    template<typename StreamT>
    accepted_type _match(stream_t &current_token, StreamT &a, const dfa_state *cur);

    void build(const std::basic_string<stream_t> &s, accepted_type accepted);
};

}
#endif