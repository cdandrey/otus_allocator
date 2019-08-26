#include "../lib/allocator.h"
#include "../lib/factorial.h"
#include <map>
#include <iostream>

static const int N = 10;

int main(int,char**)
{
    std::map<int,int> map_std;
    std::map<int,int,std::less<int>,alc::buffering_allocator<std::pair<int,int>>> map_alc;


    for (int i = 0; i < N; ++i)
        map_std.emplace(std::make_pair(i,fcl::fact(i)));

    for (int i = 0; i < N; ++i)
        map_alc.emplace(std::make_pair(i,fcl::fact(i)));

    for (const auto &p : map_std)
        std::cout << p.first << " " << p.second << std::endl;

    for (const auto &p : map_alc)
        std::cout << p.first << " " << p.second << std::endl;

    return 0;
}
