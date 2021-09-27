#include <gtest/gtest.h>

#include "onegin.h"
#include <random>
#include <cassert>
#include <time.h>

std::mt19937 rnd(time(NULL));

#define VERBOSE_COMP

int test_strings(char* str_1, char* str_2, int comp(const void*, const void*))
{
    String string_1, string_2;

    string_1.beg = str_1;
    string_1.len = strlen(str_1) - 1;
    string_2.beg = str_2;
    string_2.len = strlen(str_2) - 1;

    return comp(&string_1, &string_2);
}


TEST(Straight, Stress)
{
    String str_1, str_2;
    for (size_t i = 0; i < 1000; ++i)
    {
        str_1.len = rnd() % 1000 + 10;
        str_2.len = rnd() % 1000 + 10; 

        str_1.beg = (char*)calloc(str_1.len, sizeof(char));
        str_2.beg = (char*)calloc(str_2.len, sizeof(char));

        for(size_t j = 0; j < str_1.len - 1; ++j)
        {
            str_1.beg[j] = rnd() % 100 + 30;
        }
        str_1.beg[str_1.len - 1] = '\n';

        for(size_t j = 0; j < str_2.len; ++j)
        {
            str_2.beg[j] = rnd() % 100 + 30;
        }
        str_2.beg[str_2.len - 1] = '\n';
        
        straightComp(&str_1, &str_2);
        assert(straightComp(&str_1, &str_1) == 0);
        assert(straightComp(&str_2, &str_2) == 0);


        free(str_1.beg);
        free(str_2.beg);
    }
}

TEST(Reverse, Stress)
{
    for (size_t i = 0; i < 1000; ++i)
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
        str_1.beg[str_1.len - 1] = '\n';

        for(size_t j = 0; j < str_2.len; ++j)
        {
            str_2.beg[j] = rnd() % 100 + 30;
        }
        str_2.beg[str_2.len - 1] = '\n';
        
        reverseComp(&str_1, &str_2);
        assert(reverseComp(&str_1, &str_1) == 0);
        assert(reverseComp(&str_2, &str_2) == 0);


        free(str_1.beg);
        free(str_2.beg);
    }
}


TEST(Straight, manual)
{

    EXPECT_EQ(test_strings("ABCDEF\n", "AVDS\n", straightComp), -1);
   
    EXPECT_EQ(test_strings("Hello there, how is it going?\n", "Hello, just fine, how are you?\n", straightComp), 1);

    EXPECT_EQ(test_strings("FFF\n", "\n", straightComp), 1);

    EXPECT_EQ(test_strings("..sad00,,\n", "sadnfv.00\n", straightComp), -1);
    
    EXPECT_EQ(test_strings(".,sad\\,,\n", "..sad..\\\n", straightComp), 0);
    
    EXPECT_EQ(test_strings("\.sad\\,,\n", "][sad..\\\n", straightComp), 0);
    
    EXPECT_EQ(test_strings("s/a.d\\,,\n", "][]sad..\\\n", straightComp), 0);
}

TEST(Reverse, manual)
{

    EXPECT_EQ(test_strings("\nABCDEF \n", "\nAVDS \n", reverseComp), -1);
   
    EXPECT_EQ(test_strings("\nHello there, how is it going?\n", "\nHello, just fine, how are you?\n", reverseComp), -1);

    EXPECT_EQ(test_strings("\nFFF\n", "\n\n", reverseComp), 1); //TODO

    EXPECT_EQ(test_strings("\n..sad00,,\n", "\nsadnfv.00\n", reverseComp), -1);
    
    EXPECT_EQ(test_strings("\n.,sad\\,,\n", "\n..sad..\\\n", reverseComp), 0);
    
    EXPECT_EQ(test_strings("\n\.sad\\,,\n", "\n][sad..\\\n", reverseComp), 0);
    
    EXPECT_EQ(test_strings("\ns/a.d\\,,\n", "\n][]sad..\\\n", reverseComp), 0);
}
