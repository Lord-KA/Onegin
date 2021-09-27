#include "gqsort.h"

int compInt(const void *one, const void *other)    
{
    int a = *((int*)one);
    int b = *((int*)other);
    if (a < b)
        return -1;
    if (a > b)
        return 1;
    return 0;
}


int compChar(const void *one, const void *other) 
{
    char* a = (char*)one;
    char* b = (char*)other;
}

void testInt()
{
    int A[] = {9, 8, 7, 6, 4, 5, 0, 11};
    size_t size = 8;

    for (size_t i = 0; i < size; ++i) {
        printf("%d ", A[i]);
    }
    printf("\n");


    gqsort(A, size, sizeof(int), compInt);


    for (size_t i = 0; i < size; ++i) {
        printf("%d ", A[i]);
    }
    printf("\n");
    
}

void testChar()
{
    char** A;
    A = (char**)calloc(8, sizeof(char*));
    A[0] = "aaa";
    A[1] = "bbb";
    A[2] = "ccc";
    A[3] = "cab";
    A[4] = "cba";
    A[5] = "abc";
    A[6] = "aac";
    A[7] = "caa";

    free(A);
}


int main()
{
    testInt();
    testChar();
}
