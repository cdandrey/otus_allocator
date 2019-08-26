
#include <gtest/gtest.h>
#include "../lib/allocator.h"

TEST(allocator_test,allocator_test_init)
{
    ASSERT_TRUE(true);
}

int main (int argc,char *argv[])
{
    ::testing::InitGoogleTest(&argc,argv);

    return RUN_ALL_TESTS();
}
