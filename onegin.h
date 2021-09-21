#ifndef ONEGIN_HPP
#define ONEGIN_HPP

/*          //TODO add io.h support
#include <sys/io.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
*/

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <wchar.h>

#include "gqsort.h"

struct String {
    wchar_t* beg;
    size_t len;
} typedef String;


int straightComp(const void *first, const void *second)         //TODO
{
    wchar_t* firstBeg  = ((String*)first )->beg;
    wchar_t* secondBeg = ((String*)second)->beg;
    
    size_t firstIter  = 0;
    size_t secondIter = 0;
    size_t firstLen  = ((String*)first)->len;
    size_t secondLen = ((String*)second)->len;
    size_t len = fmin(firstLen, secondLen);

    for(;firstIter < len && secondIter < len;) {
        if (!isalpha(*(firstBeg + firstIter))) 
            ++firstIter;
        else if (!isalpha(*(secondBeg + secondIter)))
            ++secondIter;
        else { 
            if (*(secondBeg + secondIter) < *(firstBeg + firstIter)) {
                printf("First diff: %c < %c\n", *(secondBeg + secondIter), *(firstBeg + firstIter));
                return 1;
            }
            if (*(secondBeg + secondIter) > *(firstBeg + firstIter)) { 
                printf("First diff: %c < %c\n", *(secondBeg + secondIter), *(firstBeg + firstIter));
                return -1;
            }
            ++firstIter;
            ++secondIter;
        }
    }

    if (firstIter == firstLen && secondIter < secondLen)
        return -1;
    else if (secondIter == secondLen && firstIter < firstLen)
        return 1;

    return 0;
}


#endif
