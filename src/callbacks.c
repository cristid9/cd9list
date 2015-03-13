#include <string.h>
#include <stdbool.h>
#include "cd9list.h"

bool callbacks_findByAddressCmp(void *value, void *toFind, size_t size)
{
    // Since the list just stores the value to the original data we can
    // simply compare them.
    if(value == toFind) {
        return true;
    }

    return false;
}

bool callbacks_findByValueCmp(void *value, void *toFind, size_t size)
{
    if(!memcmp(value, toFind, size)) {
        return true;
    }

    return false;
}
