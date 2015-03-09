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

    node->data = data;

    // Thus we can know when we reach the end of the list.
    node->next = NULL;
    node->size = size;

    return node;
}

CD9Node *cd9list_getNode(CD9List *list, size_t index) 
{
   CD9FOREACH(list, node, i) {
        if(i == index) {
            return node;
        }
    }

    return NULL;
}

void *cd9list_get(void *self, size_t index)
{
    CD9List *list = (CD9List *)self;
    
    for(CD9Node *node = list->nodes; node != NULL; node = node->next) {
        if(index == 0) {
            return node->data;
        } 
        index--;
    }
    return NULL;
}

/*
void cd9list_appendCopy(void *self, void *data, size_t size)
{
    CD9List *list = (CD9List *)self;
    if(size == SIZE_ZERO) {
        CD9Node *newNode = cd9list_createNode(data, SIZE_ZERO);
    }
    else {
        // The user want us to make a copy of his data.
        void *holder = malloc(size);
        memmove(holder, data, size);

        CD9Node *newNode = cd9list_createNode(holder, size);
    
    }
    CD9Node *lastNode;

    if(list->nodes == NULL) {
        // If the list is empty it means that the `nodes` memebers doesn't
        // point to any node.
        list->nodes = newNode;
    }
    else {
        for(CD9Node *node = list->nodes; node != NULL; node = node->next) {
            if(node->next == NULL) {
                // That's the last node.
                lastNode = node;
            }    
        }

        // Now bind the new node to the last one.
        lastNode->next = newNode;
    }

    // We should also increase the length of the list every time we add a new
    // element.
    list->length++;
}
*/

/*
void cd9list_append(void *self, void *data)
{
    CD9List *list = (CD9List *)self;
    list->appendCopy(list, data, SIZE_ZERO);
}
*/

void cd9list_prepend(void *self, void *data)
{
    CD9List *list = (CD9List *)self;
    CD9Node *newNode = cd9list_createNode(data, SIZE_ZERO);

    if(list->nodes == NULL) {
        list->nodes = newNode;
    }
    else {
        // Used to backup the rest of the list.
        CD9Node *tmp = list->nodes;

        // Make the new node the first in the list.
        list->nodes = newNode;
        
        // Bind the new node to the rest of the list.
        list->nodes->next = tmp;
    }

    list->length++;
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

size_t cd9list_find(void *self, void *data)
{
    CD9List *list = (CD9List *)self;

    size_t i = 0;
    for(CD9Node *node = list->nodes; node != NULL; node = node->next) {
        if(node->data == data) {
            return i;
        }
        i++;
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
//    list->append         = cd9list_append;
    list->prepend        = cd9list_prepend;
    list->get            = cd9list_get;
//    list->insert         = cd9list_insert;
    list->remove         = cd9list_remove;    
    list->find           = cd9list_find;
    list->_insertCopy    = cd9list_insertCopy;

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
