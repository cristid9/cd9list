#ifndef CD9LIST_H_
#define CD9LIST_H_

#include "va_numargs.h"

/**
 * @brief Use to express the fact that the size of a node is 0.
 */
#define SIZE_ZERO 0

/*
 *  
 *
 *
 */  
typedef struct CD9Node {
    void *data;
    struct CD9Node *next;
    size_t size;
} CD9Node;

typedef struct CD9List {
    size_t length;
    CD9Node *nodes;   
    
    /**
     * @brief Call this function whenever you want to append something to the
     *        end of the list
     *
     * @param self A pointer to the list.
     * @param data The data you want to append. It must be typecsted to void *.
     *
     * @return void It doesn't return anything.
     */
    void (*append)(void *self, void *data);

    /**
     * @brief Call this function whenever you want to add a new node at the 
     *        beggining of the list.
     *
     * @param self A pointer to the current list.
     * @param data The data saved in the current node.
     *
     * @return void It doesn't return anything.
     */
    void (*prepend)(void *self, void *data);

    /**
     * @brief Call this function to get the data of the node stored at that 
     *        index.
     *
     * @param self The current list.
     * @param index The index of the node where the data you want to retrieve
     *        resides.
     *
     * @param void * The data stored at that node.
     */
    void *(*get)(void *self, size_t index);

    /**
     * @brief Use this function to insert an item at the given list. If you
     *        pass an invalid index the function won't insert the value at all.
     *
     * @param self The current list.
     * @param index The index where you want to insert the item.
     * @param data The data you want to insert.
     *
     * @return void It doesn't return anything.
     */ 
    void (*insert)(void *self, size_t index, void *data);

    /**
     * @brief Use this function to get the index of the first occurence of
     *        `data` in the list.
     *
     * @param self The current list.
     * @param data The data you are looking for.
     *
     * @return size_t The index of the element you are looking for. If there is
     *         no match for your query it will return `-1`.
     */ 
    size_t (*find)(void *self, void *data);

    /**
     * @brief Use this function to remove the element at the index specified
     *        as argument.
     *
     * @param self The current list.
     * @param index The index of the element thet you want to remove.
     *
     * @return void It doesn't return anything.
     */
    void (*remove)(void *self, size_t index);
} CD9List;

#define CD9FORECH1(list, node) for(CD9Node *node = list->nodes; node != NULL; \
                                node = node->next)

#define CD9FOREACH2(list, node, index) for(CD9Node *node = list->nodes, size_t \
                                        index; node != NULL; node = \
                                        node->next, index++)

#define CD9FOREACH(...) FOREACH##__VA_NARG__(__VA_ARGS__) 

/**
 * @brief This is the callback that will be passed when calling 
 *        `cd9list_foreach`. This callback will be called on every item on the
 *        list.
 *
 * @param item The data of a node in the list,
 * @param index The index of that item.
 * @param userData This variable will be passed to `func` at every call.
 *
 * @return void It doesn't return enything.
 */
typedef void (*CD9Callback)(void *item, size_t index, void *userData);

/*
 * @brief Use this function to create a new node in a list.
 *
 * @param data The value of the new node.
 *
 * @param size The number of bytes use by the data stored in this node. This
 *        memember will be different than `SIZE_ZERO` if you used the function
 *        `appendCopy` to and a new element in the list.
 *
 * @retur \ref CD9Node A pointer to the node that was created.
 *
 */
CD9Node *cd9list_createNode(void *data, size_t size);

/**
 * @brief Use this function to allocate memory for a new list.
 *
 * @return CD9List * Returns a newly allocated list. 
 */
CD9List *cd9list_createList();

/*
 * @brief Use this function to free the memeory allocated to a list. It will
 *        delete all its elements.
 *  
 * @param list The list you want to delete.
 *
 * @return void I doesn' return anything.
 */
void cd9list_deleteList(CD9List *list);

/**
 * @brief Use this function to free the memeory allocated to a node.
 *
 * @param node The node you want to delete.
 *
 * @return void It doesn't return anything.
 */
void cd9list_deleteNode(CD9Node *node);

/**
 * @brief Use this function to iterate over all the elements in the list.
 *
 * @param list The list you want to iterate through.
 * @param func A pointer to the function that will be called on every element
 *        on the list. Its first parameter is the data of every node(the 
 *        element) and the second parameter is the index of that element, and 
 *        the third will be passed to func at every call. Thus, the signature
 *        of the function is the following one:
 *        `void (*func)(void *item, size_t index, void *userData)`
 * @param userData This will be passed to `func` at every call. Pass `NULL`
 *        if you don't want to pass anything.       
 *
 * @return void It doesn't return anything.
 */
void cd9list_foreach(CD9List *list, CD9Callback func, void  *userData);

#endif // CD9LIST_H_
