#ifndef ONEGIN_HPP
#define ONEGIN_HPP

/**
 *  @file Header for Onegin sorting task
 */


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

static const size_t POINTER_POISON = 13;       //> Poison value for unused pointers
        
static const size_t MAX_FILENAME_SIZE = 64;    //> Max Lenght of input filename

/**
 *  @struct String
 *  @brief struct for storing pointer to c-style string and it's lenght
 */
struct String {
    /**
     *  @brief beg pointer to c-style string with text
     */
    char*  beg = (char*)POINTER_POISON;
    /**
     *  @brief lenght of the string
     */
    size_t len = -1;
} typedef String;


//==================================
//  Text struct

/**
 *  @struct Text
 *  @defgroup Text_struct
 *  @brief struct for sorting texts in many? ways
 *  @addgroup Text_struct
 *  @{
 */

struct Text {
    /**
     *  @brief pointer to allocated memory with '\n' as sideguards and actual text (data) in the middle
     */
    char  *dataWrapper = (char*)POINTER_POISON;
    /**
     *  @brief pointer to buffer for inputed text
     */
    char  *data        = (char*)POINTER_POISON;
    /**
     *  @brief lenght of text buffer
     */
    size_t dataLen     = -1;

    /**
     *  @brief pointer to Index array of String structures 
     */
    String *Index    = (String*)POINTER_POISON;

    /**
     *  @brief lenght of Index array
     */
    size_t  indexLen = -1;
} typedef Text;


/**
 *  @fn static void Text_ctor(Text *this_, char* fileName) 
 *  @brief constructor for Text struct
 *  @param this_ pointer to a Text obj
 *  @param fileName c-style string with filename
 */
static void Text_ctor(Text *this_, char* fileName) {
    FILE *inp = fopen(fileName, "r");
    if (!inp) {
        printf("Failed to open the file.\n");
        exit(0);
    }

    fseek(inp, 0, SEEK_END);
    this_->dataLen = ftell(inp);
    

    #ifndef NDEBUG
    if (this_->data != (char*)POINTER_POISON) {
        printf("WARNING: Trying to allocate memory for in-use pointer.\n");
        assert(false);
    }
    #endif

    // this_->data = (char*)calloc(this_->dataLen + 1, sizeof(char));         
    this_->dataWrapper = (char*)calloc(this_->dataLen + 2, sizeof(char));         
    
    if (!this_->dataWrapper) { 
        printf("Failed to allocate buffer memory.\n");
        assert(false);
    }

    this_->dataWrapper[0]                  = '\n';
    this_->dataWrapper[this_->dataLen + 1] = '\n';

    this_->data = this_->dataWrapper + 1;
   
    fseek(inp, 0, 0);
    fread(this_->data, sizeof(char), this_->dataLen, inp);
    this_->data[this_->dataLen] = 0;

    this_->indexLen = 0;
    for (size_t i = 0; i < this_->dataLen; ++i) {
        if (this_->data[i] == '\n') {
            ++this_->indexLen;
        }
    }

    if (this_->indexLen == 0) {   
        printf("File is empty?\n");
        free(this_->data);
        this_->data = (char*)POINTER_POISON;
        exit(0);
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

    #ifndef NDEBUG
    for (size_t i = 0; i < this_->indexLen; ++i) {
        this_->Index->beg = (char*)POINTER_POISON;
        this_->Index->len = -1;
    }
    #endif



    this_->Index[0].beg = this_->data;
    size_t indexCnt = 0;

    for (size_t i = 0; i < this_->dataLen - 1 && indexCnt < this_->indexLen - 1; ++i) {
        if (this_->data[i] == '\n') {
            printf("indexCnt = %zu\n", indexCnt);
            this_->Index[indexCnt].len = dist(this_->Index[indexCnt].beg, &this_->data[i]);
            ++indexCnt;
            this_->Index[indexCnt].beg = &this_->data[i + 1];
        }
    }
    this_->Index[indexCnt].len = dist(this_->Index[indexCnt].beg, this_->data + this_->dataLen - 1);
}

/**
 *  @fn static void Text_dumpIndex(Text *this_)
 *  @brief dumps Index obj and its contents
 *  @param this_ pointer to a Text obj
 */
static void Text_dumpIndex(Text *this_)         //TODO add cout/cerr pipes support
{
    printf("Index:\nindexLen = %zu\n\n", this_->indexLen);
    for (size_t i = 0; i < this_->indexLen; ++i) {
        char* iter = this_->Index[i].beg;
        while(*iter != '\n') {  
            printf("%c", *iter);
            ++iter;
        }
        printf("\n size = %zu\n", this_->Index[i].len);
    }
}

/**
 *  @fn static void Text_printIndex(Text *this_)
 *  @brief prints text in order specified by Index
 *  @param this_ pointer to a Text obj
 */
static void Text_printIndex(Text *this_)         //TODO add cout/cerr pipes support
{
    for (size_t i = 0; i < this_->indexLen; ++i) {
        char* iter = this_->Index[i].beg;
        while(*iter != '\n') {  
            printf("%c", *iter);
            ++iter;
        }
        printf("\n");
    }
}

/**
 *  @fn static void Text_printBuf(Text *this_)
 *  @brief prints text in order as inputed
 *  @param this_ pointer to a Text obj
 */
static void Text_printBuf(Text *this_)
{
    char tmp = this_->data[this_->dataLen];
    this_->data[this_->dataLen] = '\0';

    printf("%s", this_->data);

    this_->data[this_->dataLen] = tmp;
}

/**
 *  @fn static void Text_dtor(Text *this_)
 *  @brief Text structure destructor
 *  @param this_ pointer to a Text obj
 */
static void Text_dtor(Text *this_)
{
    #ifndef NDEBUG
    if (this_->data == (char*)POINTER_POISON || this_->Index == (String*)POINTER_POISON) {
        printf("WARNING: Trying to destroy Text with poisoned pointer.\n");
        assert(false);
    }
    #endif

    free(this_->dataWrapper);
    free(this_->Index);

    this_->data        = (char*)POINTER_POISON;
    this_->dataWrapper = (char*)POINTER_POISON;
    this_->Index     = (String*)POINTER_POISON;
}

/**
 *  @}          // end of Text_struct group
 */

//==================================
// Compair functions

/**
 *  @addgroup Comparators
 *  @{
 *  @fn int continuoslyCompare(char* firstIter, char* secondIter, int direction) 
 *  @brief generalized func for comparing c-style strings bound by '\n'
 *  @param firstIter pointer (iterator) to one c-style string 
 *  @param secondIter pointer (iterator) to other c-style string 
 *  @param direction equals 1 or -1 dependent on the intended direction of iterators
 *  @return -1 if first < second; 1 if first > second; 0 if first == second
 */
int continuoslyCompare(char* firstIter, char* secondIter, int direction) 
{
    assert(direction == 1 || direction == -1);

    while (*firstIter != '\n' && *secondIter != '\n') {
        if (!isalpha(*(firstIter))) 
            firstIter += direction;
        else if (!isalpha(*(secondIter)))
            secondIter += direction;
        else { 
            if (*(secondIter) < *(firstIter)) {
                #ifdef  VERBOSE_COMP
                printf("First diff chars: %c < %c\n", *secondIter, *firstIter);
                #endif
                return 1;
            }
            if (*(secondIter) > *(firstIter)) { 
                #ifdef  VERBOSE_COMP
                printf("First diff chars: %c > %c\n", *secondIter, *firstIter);
                #endif
                return -1;
            }
            firstIter += direction;
            secondIter += direction;
        }
    }
    while (!isalpha(*firstIter) && *firstIter != '\n')
        firstIter += direction;

    while (!isalpha(*secondIter) && *secondIter != '\n')
        secondIter += direction;

    if (*firstIter < *secondIter) {
        #ifdef  VERBOSE_COMP
        printf("Same letters diff lens: %zu < %zu\n", first->len, second->len);
        #endif
        return -1;
    }
    else if (*firstIter > *secondIter) {
        #ifdef  VERBOSE_COMP
        printf("Same letters diff lens: %zu %zu\n", first->len, second->len);
        #endif
        return 1;
    }
    
    #ifdef  VERBOSE_COMP
    printf("Equal strings\n");
    #endif

    return 0;
}

/**
 *  @fn static int straightComp(const void *first_inp, const void *second_inp)         
 *  @brief generalized func for comparing c-style strings bound by '\n'
 *  @param first_inp pointer to the first String struct
 *  @param first_inp pointer to the second String struct
 *  @return -1 if first < second; 1 if first > second; 0 if first == second
 */
static int straightComp(const void *first_inp, const void *second_inp)         
{
    String* first = (String*)first_inp;
    String* second = (String*)second_inp;

    char* firstIter  = first->beg;
    char* secondIter = second->beg;
    
    assert(firstIter  != (char*)POINTER_POISON);
    assert(secondIter != (char*)POINTER_POISON);
    
    return continuoslyCompare(firstIter, secondIter, 1);
}

/**
 *  @fn static int reverseComp(const void *first_inp, const void *second_inp)         
 *  @brief generalized func for comparing c-style strings bound by '\n' starting from the end
 *  @param first_inp pointer to the first String struct
 *  @param first_inp pointer to the second String struct
 *  @return -1 if first < second; 1 if first > second; 0 if first == second
 */
static int reverseComp(const void *first_inp, const void *second_inp)         
{
    String* first = (String*)first_inp;
    String* second = (String*)second_inp;

    assert(first->beg  != (char*)POINTER_POISON);
    assert(second->beg != (char*)POINTER_POISON);

    char* firstIter  = first->beg + first->len - 1;
    char* secondIter = second->beg + second->len - 1;
    

    return continuoslyCompare(firstIter, secondIter, -1);
}

#endif
