#ifndef _MULTI_INDEX_MAP_H_INCLUDED
#define _MULTI_INDEX_MAP_H_INCLUDED


#include <list>
#include <tuple>
#include <algorithm>


template<typename Key>
struct KeyTraits {
    template<typename Value>
    using Map = typename Key::template Map<Value>;

    template<typename Value>
    static void add(Map<Value>& m, Value v) {
        Key::add(m, v);
    }

    template<typename Value>
    static void del(Map<Value>& m, Value v) {
        Key::del(m, v);
    }
};


template<typename Key, typename Value>
using KeyMap = typename KeyTraits<Key>::template Map<Value>;


template<typename T, typename...Keys>
class MultiIndexMap {
public:
    using Container = std::list<T>;

    template<typename Key>
    using Map = KeyMap<Key, typename Container::iterator>;

    using MapTuple = std::tuple<Map<Keys>...>;

    template<typename...Args>
    void add(Args&&...args) {
        auto it = data_.emplace(data_.end(), std::forward<Args>(args)...);
        addKeys(it);
    }

    bool del(const T& v) {
        auto it = std::find(data_.begin(), data_.end(), v);
        if (data_.end() == it) {
            return false;
        }

        delKeys(it);

        data_.erase(it);

        return true;
    }

    template<size_t I>
    auto& get() {
        return std::get<I>(maps_);
    }

    template<typename Key>
    auto& get() {
        return std::get<Map<Key>>(maps_);
    }

private:

    template<typename Key>
    void addKey(typename Container::iterator it) {
        KeyTraits<Key>::add(std::get<Map<Key>>(maps_), it);
    }

    void addKeys(typename Container::iterator it) {
        char dummy[] = { (addKey<Keys>(it), '0')... };
        (void)dummy;
    }

    template<typename Key>
    void delKey(typename Container::iterator it) {
        KeyTraits<Key>::del(std::get<Map<Key>>(maps_), it);
    }

    void delKeys(typename Container::iterator it) {
        char dummy[] = { (delKey<Keys>(it), '0')... };
        (void)dummy;
    }

    Container data_;
    MapTuple maps_;
};

#endif //_MULTI_INDEX_MAP_H_INCLUDED
