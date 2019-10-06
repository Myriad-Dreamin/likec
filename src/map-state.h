

#ifndef MAP_STATE_HHH
#define MAP_STATE_HHH
#include <map>

template<typename stream_t, typename accepted_type>
struct MapState {
    typename std::enable_if<std::is_signed<accepted_type>::value>::type type_check() {};
    using cur_type = MapState<stream_t, accepted_type>;
    accepted_type accepted;

    std::map<stream_t, cur_type*> nx;
    MapState(): accepted(-1) {}
    MapState(accepted_type accepted): accepted(accepted) {}
    MapState(const cur_type &b) : accepted(b.accepted), nx(b.nx) {}
    MapState(cur_type &b) : accepted(b.accepted), nx(b.nx) {}
    MapState(cur_type &&b) {
        accepted = b.accepted;
        nx = std::move(b.nx);
    }
    ~MapState() {
        for (auto &s :nx) {
            delete s.second;
        }
    }
    bool is_accepted() { return accepted != -1; }
    static bool is_accepted(accepted_type ac) { return ac != -1; }
    const cur_type* &at(const stream_t &c) { return this->nx.at(c); }
    bool count(const stream_t &c) { return this->nx.count(c); }
};

#endif