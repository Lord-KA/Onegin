#include <gqsort.h>

void swap(void *one, void *other, size_t size) 
{
    assert(one);
    assert(other);

    for (size_t i = 0; i < size; ++i) {
          char  tmp       = ((char*)one)[i];
        ((char*)one)[i]   = ((char*)other)[i];
        ((char*)other)[i] = tmp;
    }
}

size_t dist(const void *one, const void *other)
{
    return (const char*)other - (const char*)one;
}

void* partition(void* data, size_t pos, size_t count, size_t size, int (*comp)(const void*, const void*)) 
{
    char *left = (char*)data;                        
    char *right = (char*)data + (count - 1) * size;

    void *pivot = calloc(1, size);      //TODO think how to leave calloc alone
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
