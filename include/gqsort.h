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
void swap(void *one, void *other, size_t size) ;

/**
 *  @fn size_t dist(const void *one, const void *other)
 *  @brief returns distance between pointers
 *  @param one first pointer
 *  @param other second pointer
 *  @return distance
 */
size_t dist(const void *one, const void *other);

/**
 *  @addgroup GQSort
 *  @{
 *  @fn void* partition(void* data, size_t pos, size_t count, size_t size, int (*comp)(const void*, const void*)) 
 *  @brieg part of Quick Sort alg that divides array into two over pivot 
 *  @param data pointer to the array of objects
 *  @param pos position of the pivot (dividing element)
 *  @param count len of `data`
 *  @param size size of an object
 *  @param comp compare func that returns -1 if first < second; 1 if first > second; 0 if first == second
 *  @return pointer to the middle of the division
 */
void* partition(void* data, size_t pos, size_t count, size_t size, int (*comp)(const void*, const void*));

/**
 *  @fn void gqsort(void *data, size_t count, size_t size, int (*comp)(const void*, const void*))
 *  @brief main func of Quick Sort alg
 *  @param data pointer to the array of objects
 *  @param count len of `data`
 *  @param size size of an object
 *  @param comp compare func that returns -1 if first < second; 1 if first > second; 0 if first == second
 */
void gqsort(void *data, size_t count, size_t size, int (*comp)(const void*, const void*));


#endif
