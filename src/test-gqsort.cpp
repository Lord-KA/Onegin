#include "gqsort.h"
#include <random>

#include "gtest/gtest.h"

std::mt19937 rnd(NULL);

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


TEST(GQSort, manual)
{
    int A[] = {9, 8, 7, 6, 4, 5, 0, 11};
    int B[] = {9, 8, 7, 6, 4, 5, 0, 11};
    
    size_t size = 8;


    gqsort(A, size, sizeof(int), compInt);
    gqsort(B, size, sizeof(int), compInt);
    
    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(A[i], B[i]);
    }
}

TEST(GQSort, random)
{
    for (size_t j = 0; j < 100; ++j) {

        size_t size = rnd() % 10000 + 100;

        int *A = (int*)calloc(size, sizeof(int));
        int *B = (int*)calloc(size, sizeof(int));

        for (size_t i = 0; i < size; ++i) {
            A[i] = rnd();
            B[i] = A[i];
        }

        gqsort(A, size, sizeof(int), compInt);
        qsort(B, size, sizeof(int), compInt);


        for (size_t i = 0; i < size; ++i) {
            EXPECT_EQ(A[i], B[i]);
        }

        free(A);
        free(B);
    }

}
