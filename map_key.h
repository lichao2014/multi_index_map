#ifndef _MAP_KEY_H_INCLUDED
#define _MAP_KEY_H_INCLUDED


#include <map>


template<typename Obj, typename T, T(Obj::*P)>
struct UniqueKey {
    template<typename Value>
    using Map = std::map<T, Value>;

    template<typename Value>
    static bool add(Map<Value>& m, Value v) {
        return  m.try_emplace((*v).*P, v).second;
    }

    template<typename Value>
    static bool del(Map<Value>& m, Value v) {
        return 0 != m.erase((*v).*P);
    }
};


template<typename Obj, typename T, T(Obj::*P)>
struct NonUniqueKey {
    template<typename Value>
    using Map = std::multimap<T, Value>;

    template<typename Value>
    static bool add(Map<Value>& m, Value v) {
        m.emplace((*v).*P, v);
        return true;
    }

    template<typename Value>
    static bool del(Map<Value>& m, Value v) {
        auto range = m.equal_range((*v).*P);
        for (auto i = range.first; i != range.second; ++i) {
            if (*(i->second) == *v) {
                m.erase(i);
                return true;
            }
        }

        return false;
    }
};


#endif //_MAP_KEY_H_INCLUDED
