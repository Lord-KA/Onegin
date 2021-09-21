#include "onegin.h"

int main()
{
    char str_1[] = "Helloa\n";
    char str_2[] = "Hello\n";

    String string_1, string_2;
    string_1.beg = str_1;
    string_1.len = strlen(str_1) - 1;
    string_2.beg = str_2;
    string_2.len = strlen(str_2) - 1;
    printf("%s | %d\n", string_1.beg, string_1.len);
    printf("%s | %d\n", string_2.beg, string_2.len);

    // printf("%d\n", isalpha(*(str_1 + 1)));
    printf("comp = %d\n", straightComp(&string_1, &string_2));
}
