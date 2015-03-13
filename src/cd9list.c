#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cd9list.h"

void cd9list_foreach(CD9List *list, CD9Callback func, void *userData) 
{
    size_t index = 0;
    for(CD9Node *node = list->nodes; node != NULL; node = node->next) {
        (*func)(node->data, index, userData);
        index++;
    }
}

CD9Node *cd9list_createNode(void *data, size_t size) 
{
    CD9Node *node = malloc(sizeof(CD9Node));
    if(node == NULL) { // Malloc failed.
        return NULL;
    }

    // Thus we can know when we reach the end of the list.
    if(size != SIZE_ZERO) {
        void *copy = malloc(size);
        memmove(copy, data, size);

        node->data = copy;
    }
    else {    
        node->data = data;
    }
    
    node->next = NULL;
    node->size = size;

    return node;
}

CD9Node *cd9list_getNode(CD9List *list, size_t index) 
{
   CD9FOREACH_(list, node, i) {
        if(i == index) {
            return node;
        }
    }

    return NULL;
}

void *cd9list_copyNodeData(CD9Node *node)
{
    // The user was careless, he shouldn't call this function on an empty
    // node.
    if(node->size == SIZE_ZERO) {
        return NULL; 
    }

    void *data = malloc(node->size);
    memmove(data, node->data, node->size);

    return data;
}

void *cd9list_get(void *self, size_t index)
{
    CD9List *list = (CD9List *)self;
    CD9Node *node = cd9list_getNode(list, index);
    
    return node->data;
}

void cd9list_append(void *self, void *data)
{
    CD9List *list = (CD9List *)self;
    list->_insertCopy(list, list->length, data, SIZE_ZERO);
}

void cd9list_appendCopy(void *self, void *data, size_t size)
{
    CD9List *list = (CD9List *)self;
    list->_insertCopy(list, list->length, data, size);
}

// %todo% write tests and docs for this function.
void cd9list_insert(void *self, size_t index, void *data)
{
    CD9List *list = (CD9List *)self;
    list->_insertCopy(list, index, data, SIZE_ZERO);
}

void cd9list_prepend(void *self, void *data)
{
    CD9List *list = (CD9List *)self;
    list->_insertCopy(list, 0, data, SIZE_ZERO);
}

void *cd9list_pop(void *self)
{
    CD9List *list = (CD9List *)self;
    CD9Node *node = cd9list_getNode(list, list->length - 1);

    if(node->size == SIZE_ZERO) {
        void *tmp = node->data;
        list->remove(list, list->length - 1);

        return tmp; 
    }

    void *tmp = cd9list_copyNodeData(node);
    list->remove(list, list->length - 1);

    return tmp;
}

void *cd9list_popleft(void *self)
{
    CD9List *list = (CD9List *)self;
    CD9Node *node = cd9list_getNode(list, 0);

    if(node->size == SIZE_ZERO) {
        void *tmp = node->data;
        list->remove(list, 0);

        return tmp;
    }

    void *tmp = cd9list_copyNodeData(node);
    list->remove(list, 0);

    return tmp;
}

void cd9list_insertCopy(void *self, size_t index, void *data, size_t size)
{
    CD9List *list = (CD9List *)self;

    if(index == 0) {
        CD9Node *node = cd9list_createNode(data, size);
        CD9Node *tmp = list->nodes;
        
        list->nodes = node;
        node->next = tmp;
        
        list->length++;

        return;
    }
    CD9Node *beforeDesiredNode = cd9list_getNode(list, index - 1);
    CD9Node *tmp = beforeDesiredNode->next;
    CD9Node *node = cd9list_createNode(data, size);
    
    // Adjust the links.
    beforeDesiredNode->next = node;
    node->next = tmp;

    list->length++;
}

int cd9list_remove(void *self, size_t index)
{
    CD9List *list = (CD9List *)self;
    
    if(index < 0 || index > (list->length - 1)) {
        return 0; // Not a valid index;
    } 

    if(index == 0) {
        CD9Node *toDelete = list->nodes;
        list->nodes = toDelete->next;
        cd9list_deleteNode(toDelete);
        list->length--;
    }
    else {
        CD9Node *prev = cd9list_getNode(list, index - 1);
        CD9Node *toDelete = cd9list_getNode(list, index);

        prev->next = toDelete->next;
        cd9list_deleteNode(toDelete);
        
        list->length--;
    }

    return 1; // Removed successfully.
}

int cd9list_find(void *self, void *toFind, CD9FindCallback cmp)
{
    CD9List *list = (CD9List *)self;

    CD9FOREACH_(list, node, index) {
        if(cmp(node->data, toFind, node->size)) {
            return index;
        }
    }
    return -1;
}

CD9List *cd9list_createList()
{
    CD9List *list = malloc(sizeof(CD9List));
    if(list == NULL) { // Malloc failed.
        return NULL; 
    }
    
    list->length = 0;
    list->nodes  = NULL;

    // Now bind the functions;
    list->append         = cd9list_append;
    list->prepend        = cd9list_prepend;
    list->get            = cd9list_get;
    list->insert         = cd9list_insert;
    list->remove         = cd9list_remove;    
    list->find           = cd9list_find;
    list->_insertCopy    = cd9list_insertCopy;
    list->pop            = cd9list_pop;
    list->popleft        = cd9list_popleft;
    list->appendCopy     = cd9list_appendCopy;

    return list;
}

void cd9list_deleteList(CD9List *list)
{
    CD9Node *phead = list->nodes;
    CD9Node *tmp;

    while(phead != NULL) {
        tmp = phead->next;
        free(phead);
        phead = tmp;
    }

    free(list);
}

void cd9list_deleteNode(CD9Node *node)
{
    if(node->size != SIZE_ZERO) {
        // Free the data allocated by cd9list_insertCopy.
        free(node->data);    
    }

    free(node);
}
