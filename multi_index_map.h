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
    static bool add(Map<Value>& m, Value v) {
        return Key::add(m, v);
    }

    template<typename Value>
    static bool del(Map<Value>& m, Value v) {
        return Key::del(m, v);
    }
};


template<typename Key, typename Value>
using KeyMap = typename KeyTraits<Key>::template Map<Value>;


template<typename T>
T *null() {
    return static_cast<T *>(nullptr);
}

template<typename T, typename...Keys>
class MultiIndexMap {
public:
    using Container = std::list<T>;

    template<typename Key>
    using Map = KeyMap<Key, typename Container::iterator>;

    using MapTuple = std::tuple<Map<Keys>...>;

    template<typename...Args>
    bool add(Args&&...args) {
        auto it = data_.emplace(data_.end(), std::forward<Args>(args)...);
        if (addKey(it)) {
            return true;
        }

        data_.erase(it);

        return false;
    }

    bool del(const T& v) {
        auto it = std::find(data_.begin(), data_.end(), v);
        if (data_.end() == it) {
            return false;
        }

        delKey(it);

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
    bool addKey(typename Container::iterator it, Key *) {
        return KeyTraits<Key>::add(std::get<Map<Key>>(maps_), it);
    }

    template<typename Key, typename...LeftKeys>
    bool addKey(typename Container::iterator it, Key *, LeftKeys *...) {
        return addKey(it, null<Key>()) && addKey(it, null<LeftKeys>()...);
    }

    bool addKey(typename Container::iterator it) {
        if (addKey(it, null<Keys>()...)) {
            return true;
        }

        delKey(it);

        return false;
    }

    template<typename Key>
    size_t delKey(typename Container::iterator it, Key *) {
        return KeyTraits<Key>::del(std::get<Map<Key>>(maps_), it);
    }

    template<typename Key, typename...LeftKeys>
    size_t delKey(typename Container::iterator it, Key *, LeftKeys*...) {
        return delKey<Key>(it, null<Key>()) + delKey(it, null<LeftKeys>()...);
    }

    size_t delKey(typename Container::iterator it) {
        return delKey(it, null<Keys>()...);
    }

    Container data_;
    MapTuple maps_;
};

#endif //_MULTI_INDEX_MAP_H_INCLUDED
