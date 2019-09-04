
#pragma once

namespace fcl
{
    template<int V>
    struct tfact
    {
        static const int value = V * tfact<V - 1>::value;
    };

    template<>
    struct tfact<0>
    {
        static const int value = 1;
    };

    int fact(unsigned x);

}

