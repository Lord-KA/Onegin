#ifndef GQSORT_H
#define GQSORT_H

/**
 *  @file lib header for yet another quick sort
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**
 *  @fn void swap(void *one, void *other, size_t size) 
 *  @brief generalized full swap func (extra memory requaired)
 *  @param one pointer to the first obj
 *  @param other pointer to the second obj
 *  @param size size of the objects
 */
void swap(void *one, void *other, size_t size) 
{
    void *tmp = calloc(1, size);
    memcpy(tmp, one, size);
    memcpy(one, other, size);
    memcpy(other, tmp, size);   
    free(tmp);
}

/**
 *  @fn size_t dist(const void *one, const void *other)
 *  @brief returns distance between pointers
 *  @param one first pointer
 *  @param other second pointer
 *  @return distance
 */
size_t dist(const void *one, const void *other)
{
    return (const char*)other - (const char*)one;
}

/**
 *  @addgroup GQSort
 *  @{
 *  @fn static void* partition(void* data, size_t pos, size_t count, size_t size, int (*comp)(const void*, const void*)) 
 *  @brieg part of Quick Sort alg that divides array into two over pivot 
 *  @param data pointer to the array of objects
 *  @param pos position of the pivot (dividing element)
 *  @param count len of `data`
 *  @param size size of an object
 *  @param comp compare func that returns -1 if first < second; 1 if first > second; 0 if first == second
 *  @return pointer to the middle of the division
 */
static void* partition(void* data, size_t pos, size_t count, size_t size, int (*comp)(const void*, const void*)) 
{
    char *left = (char*)data;                        
    char *right = (char*)data + (count - 1) * size;

    void *pivot = calloc(1, size);
    assert(pivot);

    memcpy(pivot, (const char*)data + pos * size, size);

    while (left <= right)
    {
        while((*comp)((void*)left, pivot) < 0)
            left += size;

        while((*comp)((void*)right, pivot) > 0)
            right -= size;
        
        if (left >= right)
            break;

        swap(left, right, size);
        left += size;
        right -= size;
    }

    free(pivot);
    return right + size;
}

/**
 *  @fn void gqsort(void *data, size_t count, size_t size, int (*comp)(const void*, const void*))
 *  @brief main func of Quick Sort alg
 *  @param data pointer to the array of objects
 *  @param count len of `data`
 *  @param size size of an object
 *  @param comp compare func that returns -1 if first < second; 1 if first > second; 0 if first == second
 */
void gqsort(void *data, size_t count, size_t size, int (*comp)(const void*, const void*))
{
    if (count <  2)
        return;
    if (count == 2) {
        if ((*comp)(data, (char*)data + size) > 0)
            swap(data, (char*)data + size, size);
        return;
    }

    void *middle = partition(data, count / 2, count, size, comp);
    gqsort(data, dist(data, middle) / size, size, comp);      
    gqsort(middle, count - dist(data, middle) / size, size, comp);
}





#endif
