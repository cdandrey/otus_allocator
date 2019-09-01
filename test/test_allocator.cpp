
#include "../lib/allocator.h"
#include "../lib/fifo.h"
#include <gtest/gtest.h>
#include <map>
#include <deque>
#include <random>
#include <iostream>

TEST(allocator_test,test_buffering_alloc)
{
    const unsigned C_NUM_TEST{1000000};
    const unsigned C_SIZE_MAP{10000};
    unsigned op;
    unsigned key;

    std::map<int,int> map_std;
    std::map<int,int,std::less<int>,alc::buffering_allocator<std::pair<int,int>> > map_alc;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<unsigned> dis_op = {60,20,20};
    std::uniform_int_distribution<unsigned> dis_key(0,C_SIZE_MAP - 1);

    for (int i = 0; i < C_NUM_TEST; ++i)
    {
        op = dis_op(gen);
        key = dis_key(gen);

        switch (op)
        {
            // emplace
            case 0:
                EXPECT_EQ(map_std.emplace(std::make_pair(key,(~key + 1))).second,map_alc.emplace(std::make_pair(key,(~key + 1))).second);
                break;

            // remove
            case 1:
                EXPECT_EQ(map_std.erase(key),map_alc.erase(key));
                break;

            // find
            case 2:
                EXPECT_EQ(map_std.find(key) != map_std.end(),map_alc.find(key) != map_alc.end());
                break;
        }
    }
    
    auto it_std = map_std.cbegin();
    auto it_alc = map_alc.cbegin();
    for (; it_std != map_std.cend() && it_alc != map_alc.cend(); ++it_std, ++it_alc)
    {
        EXPECT_EQ(it_std->first,it_alc->first);
        EXPECT_EQ(it_std->second,it_alc->second);
    }

    EXPECT_EQ(map_std.size(), map_alc.size());
}
//----------------------------------------------------------------------------------


TEST(allocator_test,test_fifo)
{
    const unsigned C_NUM_TEST{1000000};
    unsigned op;
    unsigned value;

    std::deque<int> deq;
    ffl::fifo<int> ff;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<unsigned> dis_op = {50,30,20};
    std::uniform_int_distribution<unsigned> dis_value;

    for (unsigned i = 0; i < C_NUM_TEST; ++i)
    {
        op = (deq.size()) ? dis_op(gen) : 0;

        switch (op)
        {
            // push
            case 0:
                value = dis_value(gen);
                deq.push_back(value);
                ff.push(value);
                EXPECT_EQ(deq.size(),ff.size());
                break;

            // pop
            case 1:
                EXPECT_EQ(deq.front(),ff.pop());
                deq.pop_front();
                EXPECT_EQ(deq.size(),ff.size());
                break;

            // pop_peek
            case 2:
                EXPECT_EQ(deq.front(),ff.pop_peek());
                EXPECT_EQ(deq.size(),ff.size());
                break;
        }
    }

    while (deq.size() && ff.size())
    {
        EXPECT_EQ(deq.front(),ff.pop());
        deq.pop_front();
        EXPECT_EQ(deq.size(),ff.size());
    }
}
//-------------------------------------------------------------------------


TEST(allocator_test,test_fifo_my_alloc)
{
    const unsigned C_NUM_TEST{1000000};
    unsigned op;
    unsigned value;

    std::deque<int> deq;
    ffl::fifo<int,alc::my_allocator<int>> ff;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<unsigned> dis_op = {50,30,20};
    std::uniform_int_distribution<unsigned> dis_value;

    for (unsigned i = 0; i < C_NUM_TEST; ++i)
    {
        op = (deq.size()) ? dis_op(gen) : 0;

        switch (op)
        {
            // push
            case 0:
                value = dis_value(gen);
                deq.push_back(value);
                ff.push(value);
                EXPECT_EQ(deq.size(),ff.size());
                break;

            // pop
            case 1:
                EXPECT_EQ(deq.front(),ff.pop());
                deq.pop_front();
                EXPECT_EQ(deq.size(),ff.size());
                break;

            // pop_peek
            case 2:
                EXPECT_EQ(deq.front(),ff.pop_peek());
                EXPECT_EQ(deq.size(),ff.size());
                break;
        }
    }

    while (deq.size() && ff.size())
    {
        EXPECT_EQ(deq.front(),ff.pop());
        deq.pop_front();
        EXPECT_EQ(deq.size(),ff.size());
    }
}
//----------------------------------------------------------------------------------


int main (int argc,char *argv[])
{
    ::testing::InitGoogleTest(&argc,argv);

    return RUN_ALL_TESTS();
}
