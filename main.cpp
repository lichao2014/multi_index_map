#include <cassert>

#include "multi_index_map.h"
#include "map_key.h"


struct A {
    int a = 0;
    int b = 0;

    A(int a, int b)
        :a(a)
        ,b(b)
    {}

    bool operator==(const A& rhs) const {
        return (a == rhs.a) && (b == rhs.b);
    }
};



int main()
{
    MultiIndexMap<A, UniqueKey<A, int, &A::a>, NonUniqueKey<A, int, &A::b>> m;
    auto ok = m.add(1, 2);
    assert(ok);
    ok = m.add(2, 2);
    assert(ok);
    ok = m.add(1, 3);
    assert(!ok);

    m.del(A(1, 2));

    auto& m0 = m.get<0>();
    auto r1 = m0.find(1);
    assert(r1 == m0.end());

    auto r2 = m0.find(2);
    assert(r2 != m0.end());
    assert(*r2->second == A(2, 2));


    auto& m1 = m.get<1>();
    auto r3 = m1.find(1);
    assert(r3 == m1.end());

    auto r4 = m1.find(2);
    assert(r4 != m1.end());
    assert(*r4->second == A(2, 2));

    return EXIT_SUCCESS;
}