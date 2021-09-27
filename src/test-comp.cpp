#include <gtest/gtest.h>

#include "onegin.h"
#include <random>
#include <cassert>
#include <time.h>

std::mt19937 rnd(time(NULL));

void test_strings(char* str_1, char* str_2, int result)
{
    String string_1, string_2;

    string_1.beg = str_1;
    string_1.len = strlen(str_1) - 1;
    string_2.beg = str_2;
    string_2.len = strlen(str_2) - 1;

    EXPECT_EQ(straightComp(&string_1, &string_2), result);
    EXPECT_TRUE(straightComp(&string_1, &string_1) == 0);
    EXPECT_TRUE(straightComp(&string_2, &string_2) == 0);
}


TEST(Straight, Stress)
{
    for (size_t i = 0; i < 100000; ++i)
    {
        String str_1, str_2;
        str_1.len = rnd() % 1000 + 10;
        str_2.len = rnd() % 1000 + 10; 

        str_1.beg = (char*)calloc(str_1.len, sizeof(char));
        str_2.beg = (char*)calloc(str_2.len, sizeof(char));

        for(size_t j = 0; j < str_1.len - 1; ++j)
        {
            str_1.beg[j] = rnd() % 100 + 30;
        }
        str_1.beg[str_1.len - 1] = NULL;

        for(size_t j = 0; j < str_2.len; ++j)
        {
            str_2.beg[j] = rnd() % 100 + 30;
        }
        str_2.beg[str_2.len - 1] = NULL;
        
        straightComp(&str_1, &str_2);
        assert(straightComp(&str_1, &str_1) == 0);
        assert(straightComp(&str_2, &str_2) == 0);


        free(str_1.beg);
        free(str_2.beg);
    }
}

TEST(Reverse, Stress)
{
    for (size_t i = 0; i < 100000; ++i)
    {
        String str_1, str_2;
        str_1.len = rnd() % 10000 + 100;
        str_2.len = rnd() % 10000 + 100; 

        str_1.beg = (char*)calloc(str_1.len, sizeof(char));
        str_2.beg = (char*)calloc(str_2.len, sizeof(char));

        for(size_t j = 0; j < str_1.len - 1; ++j)
        {
            str_1.beg[j] = rnd() % 100 + 30;
        }
        str_1.beg[str_1.len - 1] = NULL;

        for(size_t j = 0; j < str_2.len; ++j)
        {
            str_2.beg[j] = rnd() % 100 + 30;
        }
        str_2.beg[str_2.len - 1] = NULL;
        
        reverseComp(&str_1, &str_2);
        assert(reverseComp(&str_1, &str_1) == 0);
        assert(reverseComp(&str_2, &str_2) == 0);


        free(str_1.beg);
        free(str_2.beg);
    }
}


TEST(Straight, manual)
{
    test_strings("ABCDEF\n", "AVDS\n", -1)
   
    test_strings("Hello there, how is it going?\n", "Hello, just fine, how are you?\n", 1)

    test_strings("FFF\n", "\n", -1);

    test_strings("..sad\\,,\n", "sadnfv..\\\n", -1);
}


