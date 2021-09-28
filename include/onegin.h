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
 *  @fn void Text_ctor(Text *this_, char* fileName) 
 *  @brief constructor for Text struct
 *  @param this_ pointer to a Text obj
 *  @param fileName c-style string with filename
 */
void Text_ctor(Text *this_, char* fileName);


/**
 *  @fn void Text_dumpIndex(Text *this_)
 *  @brief dumps Index obj and its contents
 *  @param this_ pointer to a Text obj
 */
void Text_dumpIndex(Text *this_);


/**
 *  @fn void Text_printIndex(Text *this_)
 *  @brief prints text in order specified by Index
 *  @param this_ pointer to a Text obj
 */
void Text_printIndex(Text *this_);

/**
 *  @fn void Text_printBuf(Text *this_)
 *  @brief prints text in order as inputed
 *  @param this_ pointer to a Text obj
 */
void Text_printBuf(Text *this_);


/**
 *  @fn void Text_dtor(Text *this_)
 *  @brief Text structure destructor
 *  @param this_ pointer to a Text obj
 */
void Text_dtor(Text *this_);

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
int continuoslyCompare(char* firstIter, char* secondIter, int direction);


/**
 *  @fn int straightComp(const void *first_inp, const void *second_inp)         
 *  @brief generalized func for comparing c-style strings bound by '\n'
 *  @param first_inp pointer to the first String struct
 *  @param first_inp pointer to the second String struct
 *  @return -1 if first < second; 1 if first > second; 0 if first == second
 */
int straightComp(const void *first_inp, const void *second_inp);

/**
 *  @fn int reverseComp(const void *first_inp, const void *second_inp)         
 *  @brief generalized func for comparing c-style strings bound by '\n' starting from the end
 *  @param first_inp pointer to the first String struct
 *  @param first_inp pointer to the second String struct
 *  @return -1 if first < second; 1 if first > second; 0 if first == second
 *  @}          // end of Comparators group
 */
int reverseComp(const void *first_inp, const void *second_inp);

#endif
