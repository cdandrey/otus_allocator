
#include "factorial.h"

int fcl::fact(unsigned x)
{
    return (x == 0) ? 1 : x * fcl::fact(x - 1);
}
