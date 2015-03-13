#ifndef CD9LIST_H_
#define CD9LIST_H_

#include "va_numargs.h"
#include "macro_dispatcher.h"
#include <stdbool.h>

/**
 * @brief Use to express the fact that the size of a node is 0.
 */
#define SIZE_ZERO 0

/**
 * @brief This is the signature of the callback you will pass to to 
 *        `cd9list_find` in order to find an element. This function will be
 *        called on every element in the list, so `data` will have by turn
 *        the value of every element in the list.
 * 
 * @param data An element in the list.
 * @param toFind The data you want to find.
 * @param size The size of `data`.
 *
 * @return bool It returns `true` if the 2 values are equal and `false` 
 *         otherwise.
 */
typedef bool (*CD9FindCallback)(void *data, void *toFind, size_t size);

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
     * @brief Use this function when you want to append a copy of the data
     *        not just keeping a reference to it in the list. You will want to
     *        do this when the lifetime of the list will be longer than the
     *        lifetime of the data you want to append. For example, you want
     *        to append some values created in a function to a list. After the
     *        function returns, these values won't be available anymore, so the
     *        list will have a broken reference. Using this function you can
     *        avoid this kind of scenarios. This is a wrapped over 
     *        `cd9list_insertCopy`.
     *
     * @param self The current list.
     * @param data The data you want to append.
     * @param size The size of the data you want to append. This is necessary
     *        because we need to know how large should be the block of memory
     *        allocated for the copy. 
     *
     * @return void It doesn't return anything.
     */
    void (*appendCopy)(void *self, void *data, size_t size);

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
     * @brief This function is similar to \ref appendCopy, the only difference
     *        is that it appends the copy at the beginning of the list.
     *
     * @param self The current list.
     * @param data The data you want to prepend.
     * @param size The size of the data you want to prepend.
     *
     * @return void It doesn't return anything.
     */ 
    void (*prependCopy)(void *self, void *data, size_t size);
    
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
     * @brief Use thid function to get the first element from the list and to
     *        delete it.
     *
     * @param self The current list.
     *
     * @return void * The value of the first element from the list.
     *
     */ 
    void *(*popleft)(void *self);
    /**
     * @brief Use this function to insert an item at the given list. If you
     *        pass an invalid index the function won't insert the value at all.
     *        This is a wrapper for `cd9list_insertCopy`, but it doesn't make
     *        a copy of the data you want to store, it just stores the address
     *        you give to him.  
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
     * @param cmp The comparator function used to determine if the data you
     *        are looking for is equal with a data in the list.
     *
     * @return size_t The index of the element you are looking for. If there is
     *         no match for your query it will return `-1`.
     */ 
    int (*find)(void *self, void *data, CD9FindCallback cmp);

    /**
     * @brief This function is a wrapper over `cd9list_find`. It is intended
     *        to ease the finding of elements in a list which stores addresses
     *        not copies of the data.
     *
     * @param self The current list.
     * @param data The data you want to find.
     *
     * @return int See the return type of \ref `cd9list_find`.
     */ 
    int (*findByAddress)(void *self, void *data);

    /**
     * @brief Wrapper for `cd9list_find`. It is intended to make your life 
     *        easier if you need to find values in a list that stores copies,
     *        and by "copies" I mean pointers to copies.
     *
     * @param self The current list.
     * @param data The data you want to find.
     *
     * @return int See the return value for \ref `cd9list_find`.
     */ 
    int (*findByValue)(void *self, void *data);

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


/**
 * @brief Use this version of foreach to iterate over nodes in a list. All you
 *        will get is a node per iteration.
 */ 
#define CD9FOREACH_2(list, node) for(CD9Node *node = list->nodes; \
                                     node != NULL; node = node->next)
/**
 * @brief Use this version to iterate over the nodes in a list. You will get
 *        a node and it's index per iteration. Keep in mind that this macro
 *        is not intended to be used by the end user, since it exposes to 
 *        many internal things.
 */ 
#define CD9FOREACH_3(list, node, index) CD9Node *node;\
                                       size_t index; \
                                       for(node = list->nodes, index = 0; \
                                           node != NULL; node = \
                                           node->next, index++)
/**
 * @brief This version of foreach shouldn't be used by the library user 
 *        because it exposes him to some of the library internals, such as the
 *        concept of node. The user shouldn't be concerned about 
 *        implementation details.
 *
 */
#define CD9FOREACH_(...) MACRO_DISPATCHER(CD9FOREACH_, __VA_ARGS__) 

/**
 * @brief Use this macro to iterate over the values in a list. The variable
 *        `value` will a get a `void *` pointer to the each value in the list
 *        per iteration.
 */
#define CD9FOREACH2(list, value) CD9Node *node = list->nodes; \
                                for(void *value = node->data; node != NULL; \
                                    value = ((node->next != NULL) ? \
                                             node->next->data : NULL), \
                                    node = node->next)

/**
 * @brief It acts similar to \ref CD9FOREACH2, the only difference is that you
 *        will also get the indes of the `value`;
 */ 
#define CD9FOREACH3(list, value, index) CD9Node *node = list->nodes; \
                                       size_t index = 0; \
                                       for(void *value = node->data; \
                                           node != NULL; \
                                           value = ((node->next != NULL) ? \
                                                    node->next->data : NULL), \
                                           node = node->next, \
                                           index++)
/**
 * @brief This version of foreach is intended to be used byt the user because
 *        it passes the data, not the node to it, so it hides some of the
 *        library's internals.
 */
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
