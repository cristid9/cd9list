#ifndef CD9LIST_H_
#define CD9LIST_H_

#include "va_numargs.h"
#include "macro_dispatcher.h"

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
     *        end of the list. This is just a wrapper for `_insertCopy`.
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
     * @brief Use this function to delete the last element in the list and to
     *        delete it.
     *
     * @param self The current list.
     *
     * @return void * The data of the last element in the list.
     */ 
    void *(*pop)(void *self);

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
     * @return int It returns `0` if the index you've passed is invalid or `1`
     *         otherwise.
     */
    int (*remove)(void *self, size_t index);

    /**
     * @brief Normally, when you use functions like `append` or `prepend` you
     *        just add a pointer to the data. If that data is destroyed the
     *        list element will be useless, since the data it points to doesn't
     *        exist anymore. Using this function to add a new element the list
     *        will make a copy of the data itself. Thus, even if you delete
     *        the original data, it won't matter because the list element
     *        points to data allocated by itself.    
     *
     * @param self The current list.
     * @param index The position where you want to insert the new item.
     * @param value The value you want to insert.
     * @param size The size of the data you want to insert, thus the function
     *        will know how much memory should allocate when creating the copy.
     *
     * @return void It doesn't return anything.
     */ 
    void (*_insertCopy)(void *self, size_t index, void *value, size_t size);
} CD9List;

#define CD9FOREACH2(list, node) for(CD9Node *node = list->nodes; node != NULL; \
                                    node = node->next)

#define CD9FOREACH3(list, node, index) CD9Node *node;\
                                       size_t index; \
                                       for(node = list->nodes, index = 0;\
                                           node != NULL; node = \
                                           node->next, index++)

#define CD9FOREACH(...) MACRO_DISPATCHER(CD9FOREACH, __VA_ARGS__) 

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
 * @brief Use this function to get the node at the position `index`. This  
 *        function is intended to be used internally, a normal user should
 *        never use it.
 *
 * @param list The list where you wan to retrieve the node from.
 * @param index The position where the desired node is located.
 *
 * @return CD9Node * A pointer to the desired node.
 */
CD9Node *cd9list_getNode(CD9List *list, size_t index);

/**
 * @brief This function is mainly used as a helper function in `pop` and 
 *        `popleft` methods. In these functions you need to return the data
 *        of a node before deleting it. There wouldn't be any problem if the
 *        node just stores a pointer to the actual data, but if the data at 
 *        that node is a pointer to memory allocated by `cd9list_inserCopy`
 *        then if the node is deleted the data in it won't exist anymore.
 *        To fix this I should make a copy of a that node before deleting it
 *        and return it to the user. The problem is that the library is 
 *        designed in such a way that the user shouldn't be concerned by
 *        it's internals and if I return the data stored in that node then
 *        I will have a memory leak because I won't be able to free the memory
 *        occupied by node. The only solution is just to make a copy of a the
 *        node data before deleting it. And that is the purpose of this 
 *        function.
 *
 * @brief node The node whose data you want to copy.
 *
 * @brief CD9Node * A of the `node`'s data.
 */ 
void *cd9list_copyNodeData(CD9Node *node);

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
