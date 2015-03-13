#ifndef CALLBACKS_H__
#define CALLBACKS_H__

/**
 * @brief This is an already implemented comparator for the function 
 *        `cd9list_find`. It is intended to be used as an internal comparator
 *        for the function `cd9list_findByAddress`.
 */
bool callbacks_findByAddressCmp(void *value, void *toFind, size_t size);


#endif // CALLBACKS_H__
