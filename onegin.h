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
#include <assert.h>

#include "gqsort.h"

static const size_t POINTER_POISON = 13;

static const size_t MAX_FILENAME_SIZE = 64;


struct String {
    char*  beg = (char*)POINTER_POISON;
    size_t len = -1;
} typedef String;


//==================================
//  Text struct

struct Text {
    char  *data = (char*)POINTER_POISON;
    size_t dataLen = -1;

    String *Index = (String*)POINTER_POISON;
    size_t indexLen = -1;
} typedef Text;


static void Text_ctor(Text *this_, char* fileName) {
    FILE *inp = fopen(fileName, "r");
    if (!inp) {
        printf("Failed to open the file.\n");
        assert(false);          //TODO think of something better here
    }

    fseek(inp, 0, SEEK_END);
    this_->dataLen = ftell(inp);
    

    #ifndef NDEBUG
    if (this_->data != (char*)POINTER_POISON) {
        printf("WARNING: Trying to allocate memory for in-use pointer.\n");
        assert(false);
    }
    #endif

    this_->data = (char*)calloc(this_->dataLen + 1, sizeof(char));         //TODO free buffer properly
    
    if (!this_->data) { //TODO check what calloc returns on failure
        printf("Failed to allocate buffer memory.\n");
        assert(false);
    }
    

    fseek(inp, 0, 0);
    fread(this_->data, sizeof(char), this_->dataLen, inp);
    this_->data[this_->dataLen] = 0;

    this_->indexLen = 0;
    for (size_t i = 0; i < this_->dataLen; ++i) {
        if (this_->data[i] == L'\n') {
            ++this_->indexLen;
        }
    }

    if (this_->indexLen == 0) {   //TODO 
        printf("File is empty?\n");
        free(this_->data);
        this_->data = (char*)POINTER_POISON;
        assert(false);
    }

    
    #ifndef NDEBUG
    if (this_->Index != (String*)POINTER_POISON) {
        printf("WARNING: Trying to allocate memory for in-use pointer.\n");
        assert(false);
    }
    #endif

    this_->Index = (String*)calloc(this_->indexLen, sizeof(String));
    
    if (!this_->Index) { //TODO check what calloc returns on failure
        printf("Failed to allocate buffer memory.\n");
        assert(false);
    }


    this_->Index[0].beg = this_->data;
    size_t indexCnt = 0;

    for (size_t i = 0; i < this_->dataLen && indexCnt < this_->indexLen - 1; ++i) {
        if (this_->data[i] == '\n') {
            printf("indexCnt = %zu\n", indexCnt);
            this_->Index[indexCnt].len = dist(this_->Index[indexCnt].beg, &this_->data[i]);// / sizeof(char); //TODO find why is this constant 
            ++indexCnt;
            this_->Index[indexCnt].beg = &this_->data[i + 1];
        }
    }
    this_->Index[indexCnt].len = dist(this_->Index[indexCnt].beg, this_->data + this_->dataLen - 1);
}

static void Text_dumpIndex(Text *this_)         //TODO add cout/cerr pipes support
{
    for (size_t i = 0; i < this_->indexLen; ++i) {
        char* iter = this_->Index[i].beg;
        while(*iter != '\n') {  
            printf("%c", *iter);
            ++iter;
        }
        printf("\n size = %zu\n", this_->Index[i].len);
    }
}

static void Text_dtor(Text *this_)
{
    #ifndef NDEBUG
    if (this_->data == (char*)POINTER_POISON || this_->Index == (String*)POINTER_POISON) {
        printf("WARNING: Trying to destroy Text with poisoned pointer.\n");
        assert(false);
    }
    #endif

    free(this_->data);
    free(this_->Index);

    this_->data = (char*)POINTER_POISON;
    this_->Index = (String*)POINTER_POISON;
}

//==================================
// Compair functions

int straightComp(const void *first, const void *second)         //TODO
{
    char* firstBeg  = ((String*)first )->beg;
    char* secondBeg = ((String*)second)->beg;
    
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
                // printf("First diff: %c < %c\n", *(secondBeg + secondIter), *(firstBeg + firstIter));
                return 1;
            }
            if (*(secondBeg + secondIter) > *(firstBeg + firstIter)) { 
                // printf("First diff: %c < %c\n", *(secondBeg + secondIter), *(firstBeg + firstIter));
                return -1;
            }
            ++firstIter;
            ++secondIter;
        }
    }

    if (firstLen < secondLen)
        return -1;
    else if (firstLen > secondLen)
        return 1;

    return 0;
}


#endif
