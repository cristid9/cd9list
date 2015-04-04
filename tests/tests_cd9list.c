#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <cd9/cd9list.h>
#include "minunit.h"

int tests_run = 0;
  
static char *test_createNode() 
{
    const char *data = "foo";
    CD9Node *node    = cd9list_createNode(data, SIZE_ZERO);

    mu_assert("[test_createNode] Error allocating the list", node != NULL);
    mu_assert("[test_createNode] Data is not stored properly in a node", 
        (char *)node->data == data );
    mu_assert("[test_createNode] Next node is not NULL", node->next == NULL);

    cd9list_deleteNode(node);

    return 0;
}

static char *test_createList()
{
    CD9List *list = cd9list_createList();

    mu_assert("[test_createList] List is empty", list != NULL);
    mu_assert("[test_createList] Default length is not 0", list->length == 0);
    mu_assert("[test_createList] List nodes are not null", 
              list->nodes == NULL);

    cd9list_deleteList(list);

    return 0;
}

static char *test_getNode()
{
    const char *data[] = {"foo", "bar", "baz"};
    
    CD9List *list = cd9list_createList();
    CD9Node *node = cd9list_createNode(data[0], SIZE_ZERO);

    CD9Node *node1 = cd9list_createNode(data[0], SIZE_ZERO);
    CD9Node *node2 = cd9list_createNode(data[1], SIZE_ZERO);
    CD9Node *node3 = cd9list_createNode(data[2], SIZE_ZERO);    

    node1->next = node2;
    node2->next = node3;

    list->nodes = node1;

    CD9Node *queryNode = cd9list_getNode(list, 0);

    mu_assert("[test_getNode] Node data is wrong", 
              node->data == queryNode->data);

    cd9list_deleteNode(node);
    cd9list_deleteList(list);

    return 0;
}

static char *test_insertCopy()
{
    char *backup[] = {"foo", "bar", "baz"};
    char *data[]   = {"foo", "bar", "baz"};
    
    CD9List *list = cd9list_createList();

    for(int i = 0; i < 3; i++) {
        list->_insertCopy(list, list->length, data[i], strlen(data[i]) + 1);
    }

    mu_assert("[test_inserCopy] The length of the list was not set properly\n", 
               list->length == 3);

    // Delete the original data.
    memset(data, 0, 3 * (strlen(data[0]) + 1));

    for(int i = 0; i < 3; i++) {
        CD9Node *node = cd9list_getNode(list, i);
        mu_assert("[test_insertCopy] Value was not inserted properly",
                  !strcmp((char *)node->data, backup[i]));
    }

    cd9list_deleteList(list);

    return 0;
}

static char *test_append()
{
    const char *data[] = {"foo", "bar", "baz"};
    CD9List *list      = cd9list_createList();    

    for(int i = 0; i < 3; i++) {
        list->append(list, data[i]);
    }

    CD9FOREACH_(list, node, index) {
        mu_assert("[test_append] Data was not appended properly", 
                  (char *)node->data == data[index]);
    }

    mu_assert("[test_append] List length was not set properly", 
              list->length == 3);

    cd9list_deleteList(list);

    return 0;
}

static char *test_prepend()
{
    const char *data[] = {"bar", "baz", "bax"};
    const char *first  = "foo";
    
    CD9List *list = cd9list_createList();

    for(int i = 0; i < 3; i++) {
        list->append(list, data[i]);
    }

    list->prepend(list, first);

    mu_assert("[test_prepend] List length was not set properly", 
              list->length == 4);

    CD9Node *node = cd9list_getNode(list, 0);

    mu_assert("[test_prepend] Element was not prepended properly", 
              node->data == (char *)first);

    cd9list_deleteList(list);

    return 0;
}

static char *test_get()
{
    const char *dataAtIndex0 = "foo";
    const char *dataAtIndex1 = "bar";
    const char *dataAtIndex2 = "baz";

    CD9List *list = cd9list_createList();
    
    list->append(list, dataAtIndex0);
    list->append(list, dataAtIndex1);
    list->append(list, dataAtIndex2);

    mu_assert("[test_get] The index doesn't point to the right data",
        (char *)list->get(list, 0) == dataAtIndex0);
    
    mu_assert("[test_get] The index doesn't point to the right data",
        (char *)list->get(list, 1) == dataAtIndex1);

    mu_assert("[test_get] The index doesn't point to the right data",
        (char *)list->get(list, 2) == dataAtIndex2);

    cd9list_deleteList(list);

    return 0;
}

static char *test_foreach2()
{
    const char *data[] = {"foo", "bar", "baz"};
    CD9List *list      = cd9list_createList();

    for(int i = 0; i < 3; i++) {
        list->append(list, data[i]);
    }

    size_t i = 0;
    CD9FOREACH(list, value) {
        mu_assert("[test_foreach2] The data provided is not correct", 
                  (char *)value == data[i]); 
        i++;
    }
  
    // We will run the previus code again just to see if multple for loops
    // can be run in the same scope, without changing the name of the loop's
    // variables.

    i = 0;
    CD9FOREACH(list, value) {
        mu_assert("[test_foreach2] The data provided is not correct in thei"
                  "second loop in the same scope", 
                  (char *)value == data[i]); 
        i++;
    }

    cd9list_deleteList(list);

    return 0;

}

static char *test_foreach3()
{
    const char *data[] = {"foo", "bar", "baz"};
    CD9List *list      = cd9list_createList();

    for(int i = 0; i < 3; i++) {
        list->append(list, data[i]);
    }

    CD9FOREACH(list, value, i) {
        mu_assert("[test_foreach3] The data provided is not correct", 
                  (char *)value == data[i]);
    }

    // We will run the piece of code from the above again in order to see
    // if there are any problems when there are 2 loops in the same scope.


    CD9FOREACH(list, value, i) {
        mu_assert("[test_foreach3] The data provided is not correct", 
                  (char *)value == data[i]);
    }

    cd9list_deleteList(list);

    return 0;
}

static char *test_remove()
{
    const char *data[] = {"foo", "bar", "baz", "bax", "bay"};    
    int status;
    
    CD9List *list = cd9list_createList();

    for(int i = 0; i < 5; i++) {
        list->_insertCopy(list, list->length, data[i], SIZE_ZERO);
    }

    status = list->remove(list, 0);
    status = list->remove(list, 1); // The middle one.
    status = list->remove(list, 2); // The last one.

    CD9Node *node1 = cd9list_getNode(list, 0);
    CD9Node *node2 = cd9list_getNode(list, 1);

    mu_assert("[test_remove] List element at index 0 is not equal to bar", 
              (char *)node1->data == data[1]);
    
    mu_assert("[test_remove] List element at index 1 is not equal to bax", 
              (char *)node2->data == data[3]);

    mu_assert("[test_remove] List length was not set propelry after the \
              removal of some elements", list->length == 2);

    mu_assert("[test_remove] Returned wrong status code", status == 1);

    status = list->remove(list, 3); // There is not element 3, error expected.

    mu_assert("[test_remove] Should've returned error code", status == 0);

    cd9list_deleteList(list);

    return 0;
}

static char *test_copyNodeData()
{
    const char *data = "foo";
    CD9Node *node    = cd9list_createNode(data, 4);
    void *copy       = cd9list_copyNodeData(node);

    cd9list_deleteNode(node);

    mu_assert("[test_copyNodeData] The node data was not copied properly", 
              !strcmp(copy, data));

    free(copy);

    return 0;
}

static char *test_pop()
{
    const char *data = "foo";

    CD9List *list = cd9list_createList();

    list->append(list, data);
    list->_insertCopy(list, list->length, data, 4);

    void *ptrToCopy = list->pop(list);
    void *ptrToData = list->pop(list);

    mu_assert("[test_pop] The size of the list was not set properly", 
              list->length == 0);
    cd9list_deleteList(list);
    
    mu_assert("[test_pop] The copy and the string foo are not equal", 
              !strcmp(ptrToCopy, data));

    mu_assert("[test_pop] Pop returned a pointer to the wrong thing", 
              (char *)ptrToData == data);
   
    free(ptrToCopy);

    return 0;
}

static char *test_popleft()
{
    const char *data = "foo";
    CD9List *list    = cd9list_createList();

    list->append(list, data);
    list->_insertCopy(list, list->length, data, 4);

    void *ptrToData = list->popleft(list);
    void *ptrToCopy = list->popleft(list);

    mu_assert("[test_popleft] The size of the list was not set properly", 
              list->length == 0);
    
    cd9list_deleteList(list);
    
    mu_assert("[test_popleft] Popleft returned a pointer to the wrong thing", 

              (char *)ptrToData == data);
    mu_assert("[test_popleft] The copy and the string foo are not equal", 
              !strcmp(ptrToCopy, data));

    free(ptrToCopy);

    return 0;
}

static char *test_insert()
{
    const char *toInsert[]    = {"head", "middle", "end"};
    const char *initialData[] = {"foo", "bar", "baz"};

    CD9List *list = cd9list_createList();

    for(int i = 0; i < 3; i++) {
        list->append(list, initialData[i]);
    }

    list->insert(list, 0, toInsert[0]);
    list->insert(list, 2, toInsert[1]);
    list->insert(list, list->length, toInsert[2]);

    mu_assert("[test_insert] The list length was not set properly", 
              list->length == 6);
    mu_assert("[test_insert] toInsert[0] was not inserted properly", 
              (char *)list->get(list, 0) == toInsert[0]);
    mu_assert("[test_insert] toInsert[1] was not inserted properly", 
              (char *)list->get(list, 2) == toInsert[1]);
    mu_assert("[test_insert] toInsert[2] was not inserted properly", 
              (char *)list->get(list, list->length - 1) == toInsert[2]);

    cd9list_deleteList(list);

    return 0;
}

static bool test_find_cmp(const void *data, const void *toFind, size_t size)
{
    // In this case we can compare the addreses since the list stores the 
    // addresses to the same thing. 
    if(data == toFind) {
        return true;
    }

    return false;
}

static char *test_find()
{
    const char *data[] = {"foo", "bar", "baz"};
    CD9List *list      = cd9list_createList();

    for(int i = 0; i < 3; i++) {
        list->append(list, data[i]);
        mu_assert("[test_find] The index returned by find is wrong", 
                  list->find(list, data[i], test_find_cmp) == i);
    }

    cd9list_deleteList(list);
    
    return 0;
}

static char *test_appendCopy()
{
    const char *data[]   = {"foo", "bar", "baz"};
    const char *backup[] = {"foo", "bar", "baz"};

    CD9List *list = cd9list_createList();

    for(int i = 0; i < 3; i++) {
        list->appendCopy(list, data[i], 4);
    }

    CD9FOREACH(list, value, index) {
        mu_assert("[test_appendCopy] The copies were not appended properly", 
                  !memcmp(value, backup[index], 4));
    }

    cd9list_deleteList(list);

    return 0;

}

static char *test_prependCopy()
{
    const char *data[]   = {"foo", "bar", "baz"};
    const char *backup[] = {"foo", "bar", "baz"};

    CD9List *list = cd9list_createList();

    for(int i = 0; i < 3; i++) {
        list->prependCopy(list, data[i], 4);
    }

    int i = 2;
    CD9FOREACH(list, data) {
        mu_assert("[test_prependCopy] The copies were not prepended properly", 
                  !memcmp(data, backup[i], 4));
        i--;
    }

    cd9list_deleteList(list);

    return 0;
}

static char *test_findByAddress()
{
    const char *data[] = {"foo", "bar", "baz"};

    CD9List *list = cd9list_createList();

    for(int i = 0; i < 3; i++) {
        list->append(list, data[i]);
        mu_assert("[test_findByAddress] The index is not correct",
                  list->findByAddress(list, data[i]) == i);
    }

    cd9list_deleteList(list);

    return 0;
}

static char *test_findByValue()
{
    const char *data[]   = {"foo", "bar", "baz"};
    const char *backup[] = {"foo", "bar", "baz"};
    
    CD9List *list = cd9list_createList();

    for(int i = 0; i < 3; i++) {
        list->appendCopy(list, data[i], 4);
        mu_assert("[test_findByValue] The index is wrong", 
                  list->findByValue(list, backup[i]) == i);
    }

    cd9list_deleteList(list);

    return 0;
}

static char *test_copy()
{
    const char *data[] = {"foo", "bar", "baz"};
    CD9List *list      = cd9list_createList();

    for(int i = 0; i < 3; i++) {
        list->append(list, data[i]);
    }

    CD9List *copyList = list->copy(list);

    // Test the version that stores references.
    for(int i = 0; i < 3; i++) {
        mu_assert("[test_copy] The pointers are not equal",
                  copyList->get(copyList, i) == list->get(list, i));
    }

    cd9list_deleteList(list);
    cd9list_deleteList(copyList);
    
    // Now test the version that stores values.
    list = cd9list_createList();
    
    for(int i = 0; i < 3; i++) {
        list->appendCopy(list, data, 4);
    }

    copyList = list->copy(list);

    for(int i = 0; i < 3; i++) {
        mu_assert("[test_copy] The values were not stored properly",
                  !memcmp(list->get(list, i), copyList->get(copyList, i), 4));
    }

    cd9list_deleteList(list);
    cd9list_deleteList(copyList);

    return 0;
}

static char *test_concat()
{
    const char *data1[] = {"foo", "bar", "baz"};
    const char *data2[] = {"bax", "bay", "bad"};
    
    CD9List *list1 = cd9list_createList();
    for(int i = 0; i < 3; i++) {
        list1->append(list1, data1[i]);
    }

    CD9List *list2 = cd9list_createList();
    for(int i = 0; i < 3; i++) {
        list2->append(list2, data2[i]);
    }

    CD9List *result = cd9list_concat(list1, list2);

    for(int i = 0; i < 3; i++) {
        mu_assert("[test_concat] First half is not equal to list1",
                  result->get(result, i) == list1->get(list1, i));
    }

    for(int i = 3, j = 0; i < 6; i++, j++) {
        mu_assert("[test_concat] Second half is not equal to list2",
                  result->get(result, i) == list2->get(list2, j));
    }

    cd9list_deleteList(list1);
    cd9list_deleteList(list2);
    cd9list_deleteList(result);

    return 0;
}

static char *test_slice()
{
    const char *data[] = {"foo", "bar", "baz", "biz"};
    CD9List *list      = cd9list_createList();

    for(int i = 0; i < 4; i++) {
        list->append(list, data[i]);
    }

    CD9List *firstTwo = list->slice(list, 0, 2, 1);

    for(int i = 0; i < 2; i++) {
        mu_assert("[test_slice] Frist slice was generated wrong", 
                  firstTwo->get(firstTwo, 0) == list->get(list, 0));
    }

    cd9list_deleteList(firstTwo);

    CD9List *middle = list->slice(list, 1, -1, 1);

    for(int i = 1, j = 0; i < 3; i++, j++) {
        mu_assert("[test_slice] The middle slice is generated wrong", 
                  list->get(list, i) == middle->get(middle, j));
    }

    cd9list_deleteList(middle);

    CD9List *lastTwo = list->slice(list, -2, 0, 1);

    for(int i = 2, j = 0; i < 4; i++, j++) {
        mu_assert("[test_slice] The last slice is wrong",
                  lastTwo->get(lastTwo, j) == list->get(list, i));
    }

    cd9list_deleteList(lastTwo);
    
    CD9List *twoByTwo = list->slice(list, 0, 0, 2);
    
    for(int i = 0, j = 0; i < 4; i += 2, j++) {
        mu_assert("[test_slice] The two by two slice is wrong",
                  list->get(list, i) == twoByTwo->get(twoByTwo, j));
    }

    cd9list_deleteList(twoByTwo);
    cd9list_deleteList(list);

    return 0;
}

static char *test_reverse()
{
    const char *data[] = {"foo", "bar", "baz", "biz"};
    CD9List *list      = cd9list_createList();

    for(int i = 0; i < 4; i++) {
        list->append(list, data[i]);
    }

    CD9List *reversed = list->copy(list);
    reversed->reverse(reversed);
    
    for(int i = 3, j = 0; i >= 0; i--, j++) {
        mu_assert("[test_reverse] The list was not reversed properly", 
                  list->get(list, i) == reversed->get(reversed, j));
    }

    cd9list_deleteList(reversed);
    cd9list_deleteList(list);
    
    return 0;
}

static int test_sort_int_cmp(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}

static int test_sort_string_cmp(const void *a, const void *b)
{
    return strcmp((char *)a, (char *)b);
}

static char *test_sort()
{
    const int firstSet[]          = {4, 3, 2, 1, 5, 6};
    const int expectedResult[]    = {1, 2, 3, 4, 5, 6};

    const char *secondSet[]       = {"d", "a", "b", "c"};
    const char *expectedResult2[] = {"a", "b", "c", "d"};

    CD9List *firstList = cd9list_createList();
    for(int i = 0; i < 6; i++) {
        firstList->append(firstList, &firstSet[i]);
    }

    firstList->sort(firstList, test_sort_int_cmp);
    CD9FOREACH(firstList, val, index) {
        mu_assert("[test_sort] Items are not sorted properly in first set",
                  *(int *)val == expectedResult[index]);
    }

    cd9list_deleteList(firstList);

    CD9List *secondList = cd9list_createList();
    for(int i = 0; i < 4; i++) {
        secondList->append(secondList, secondSet[i]);
    }
    // This is because I can't use multiple CD9FOREACH loops in the same 
    // functions. 
    
    secondList->sort(secondList, test_sort_string_cmp);
    CD9FOREACH(secondList, val, index) {
        mu_assert("[test_sort] The string list was not sorted properly",
                  (char *)val == expectedResult2[index]);
    }
    

    cd9list_deleteList(secondList);

    return 0;
}

static bool test_filter_cmp(const void *item, const void *data, size_t size)
{
    if((char *)item == (char *)data) {
        return true;
    }
    
    return false;
}

static char *test_filter()
{
    const char *data[] = {"good", "bad"};
    CD9List *list      = cd9list_createList();
    
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 2; j++) {
            list->append(list, data[j]);    
        }
    }

    CD9List *filteredList = list->filter(list, data[1], test_filter_cmp);
    mu_assert("x", filteredList->length == 3);
    for(int i = 0; i < 3; i++) {
        mu_assert("[test_filter] List is not filtered properly", 
                  filteredList->get(filteredList, i) == data[0]);
    }

    cd9list_deleteList(list);
    cd9list_deleteList(filteredList);

    return 0;
}

static char *test_filterByValue()
{
    const char *data[] = {"good", "bad"};
    CD9List *list      = cd9list_createList();

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 2; j++) {
            list->append(list, data[j]);
        }
    }

    CD9List *filtered = list->filterByValue(list, data[1]);
    for(int i = 0; i < 3; i++) {
        mu_assert("[test_filterByValue] The list was not filtered properly", 
                  filtered->get(filtered, i) == data[0]);
    }

    cd9list_deleteList(filtered);
    cd9list_deleteList(list);
    
    return 0;
}

static char *test_filterBySet()
{
    const char *data[]       = {"a", "b", "c"};
    const char *filterData[] = {data[1], data[2]};

    
    CD9List *filterList = cd9list_createList();
    CD9List *list       = cd9list_createList();

    for(int i = 0; i < 2; i++) {
        filterList->append(filterList, filterData[i]);
    }

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            list->append(list, data[j]);
        }
    }

    CD9List *filtered = list->filterBySet(list, filterList);
    
    for(int i = 0; i < 3; i++) {
        mu_assert("[test_filterBySet] The list was not filtered properly", 
                  filtered->get(filtered, i) == data[0]);
    }

    cd9list_deleteList(filterList);
    cd9list_deleteList(list);
    cd9list_deleteList(filtered);

    return 0;
}

static char *all_tests() 
{
    mu_run_test(test_createNode);
    mu_run_test(test_createList);
    mu_run_test(test_append);
    mu_run_test(test_prepend);
    mu_run_test(test_get);
    mu_run_test(test_getNode);
    mu_run_test(test_insertCopy);
    mu_run_test(test_remove);
    mu_run_test(test_append);
    mu_run_test(test_copyNodeData);
    mu_run_test(test_pop);
    mu_run_test(test_popleft);
    mu_run_test(test_insert);
    mu_run_test(test_foreach2);
    mu_run_test(test_foreach3);
    mu_run_test(test_find);
    mu_run_test(test_appendCopy);
    mu_run_test(test_prependCopy);
    mu_run_test(test_findByAddress);
    mu_run_test(test_findByValue);
    mu_run_test(test_copy);
    mu_run_test(test_concat);
    mu_run_test(test_slice);
    mu_run_test(test_reverse);
    mu_run_test(test_sort);
    mu_run_test(test_filter);
    mu_run_test(test_filterByValue);
    mu_run_test(test_filterBySet);

    return 0;
}
 
int main(int argc, char **argv) 
{
    char *result = all_tests();
    
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);
    
    return result != 0;
}    
