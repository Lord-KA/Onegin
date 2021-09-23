#include "onegin.h"
#include <random>
#include <cassert>

std::mt19937 rnd(179);

void stressTest() //TODO
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
        
        // printf("string_1 = %s\nstring_2 = %s\n", str_1.beg, str_2.beg);
        straightComp(&str_1, &str_2);
        assert(straightComp(&str_1, &str_1) == 0);
        assert(straightComp(&str_2, &str_2) == 0);


        free(str_1.beg);
        free(str_2.beg);
    }
}


int main()
{
    char str_1[] = "Helloa  \n";
    char str_2[] = "Hello addsaf\n";

    String string_1, string_2;
    string_1.beg = str_1;
    string_1.len = strlen(str_1) - 1;
    string_2.beg = str_2;
    string_2.len = strlen(str_2) - 1;
    printf("%s | %d\n", string_1.beg, string_1.len);
    printf("%s | %d\n", string_2.beg, string_2.len);

    // printf("%d\n", isalpha(*(str_1 + 1)));
    printf("comp = %d\n", straightComp(&string_1, &string_2));


    stressTest();
}


