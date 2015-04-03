#ifndef CALLBACKS_H__
#define CALLBACKS_H__

#include <stdbool.h>

/**
 * @brief This is an already implemented comparator for the function 
 *        `cd9list_find`. It is intended to be used as an internal comparator
 *        for the function `cd9list_findByAddress`.
 */
bool callbacks_findByAddressCmp(const void *value, 
                                const void *toFind, 
                                size_t     size);

/**
 * @brief An already implemented comparator intended to be used by
 *        `cd9list_findByValue`. 
 */ 
bool callbacks_findByValueCmp(const void *value, 
                              const void *toFind, 
                              size_t     size);

#endif // CALLBACKS_H__
