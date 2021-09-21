#include "gqsort.h"

int comp(const void *one, const void *other)
{
    int a = *((int*)one);
    int b = *((int*)other);
    if (a < b)
        return -1;
    if (a > b)
        return 1;
    return 0;
}


int main()
{
    int A[] = {9, 8, 7, 6, 4, 5, 0, 11};
    size_t size = 8;

    for (size_t i = 0; i < size; ++i) {
        printf("%d ", A[i]);
    }
    printf("\n");


    gqsort(A, size, sizeof(int), comp);


    for (size_t i = 0; i < size; ++i) {
        printf("%d ", A[i]);
    }
    printf("\n");
    
}
