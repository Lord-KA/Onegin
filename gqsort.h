#ifndef GQSORT_H
#define GQSORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(void *one, void *other, size_t size) 
{
    void *tmp = calloc(1, size);
    memcpy(tmp, one, size);
    memcpy(one, other, size);
    memcpy(other, tmp, size);   
    free(tmp);
}

size_t dist(const void *one, const void *other)
{
    return (const char*)other - (const char*)one;
}


static void* partition(void* data, size_t pos, size_t count, size_t size, int (*comp)(const void*, const void*)) 
{
    char *left = (char*)data;                        
    char *right = (char*)data + (count - 1) * size;
    void *pivot = calloc(1, size);
                                    //TODO check if pivot was allocated
                                    //
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
