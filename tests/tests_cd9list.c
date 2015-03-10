#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"
#include "../src/cd9list.h"

int tests_run = 0;
  
static char *test_createNode() 
{
    const char *data = "foo";
    CD9Node *node = cd9list_createNode((void *)data, SIZE_ZERO);

    mu_assert("Error allocating the list", node != NULL);
    mu_assert("Data is not stored properly in a node", 
        (char *)node->data == data );
    mu_assert("Next node is not NULL", node->next == NULL);

    return 0;
}

static char *test_createList()
{
    CD9List *list = cd9list_createList();

    mu_assert("List is empty", list != NULL);
    mu_assert("Default length is not 0", list->length == 0);
    mu_assert("List nodes are not null", list->nodes == NULL);

    return 0;
}

static char *test_getNode()
{
    const char *data[] = {"foo", "bar", "baz"};
    
    CD9List *list = cd9list_createList();
    CD9Node *node = cd9list_createNode((void *)data[0], SIZE_ZERO);

    CD9Node *node1 = cd9list_createNode((void *)data[0], SIZE_ZERO);
    CD9Node *node2 = cd9list_createNode((void *)data[1], SIZE_ZERO);
    CD9Node *node3 = cd9list_createNode((void *)data[2], SIZE_ZERO);    

    node1->next = node2;
    node2->next = node3;

    list->nodes = node1;

    CD9Node *queryNode = cd9list_getNode(list, 0);

    mu_assert("[test_getNode]", node->data == queryNode->data);

    return 0;
}

static char *test_insertCopy()
{
    char *backup[] = {"foo", "bar", "baz"};
    
    char *data[] = {"foo", "bar", "baz"};
    
    CD9List *list = cd9list_createList();

    for(int i = 0; i < 3; i++) {
        list->_insertCopy(list, list->length, (void *)data[i], 
                          strlen(data[i]) + 1);
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

    CD9List *list = cd9list_createList();    

    for(int i = 0; i < 3; i++) {
        list->append(list, (void *)data[i]);
    }

    CD9FOREACH(list, node, index) {
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
    const char *first = "foo";
    
    CD9List *list = cd9list_createList();

    for(int i = 0; i < 3; i++) {
        list->append(list, (void *)data[i]);
    }

    list->prepend(list, (void *)first);

    mu_assert("[test_prepend] List length was not set properly", 
              list->length == 4);

    CD9Node *node = cd9list_getNode(list, 0);

    mu_assert("[test_prepend] Element was not prepended properly", 
              node->data == (char *)first);

    return 0;
}

static char *test_get()
{
    const char *dataAtIndex0 = "foo";
    const char *dataAtIndex1 = "bar";
    const char *dataAtIndex2 = "baz";

    CD9List *list = cd9list_createList();
    
    list->append(list, (void *)dataAtIndex0);
    list->append(list, (void *)dataAtIndex1);
    list->append(list, (void *)dataAtIndex2);

    mu_assert("The index doesn't point to the right data",
        (char *)list->get(list, 0) == dataAtIndex0);
    
    mu_assert("The index doesn't point to the right data",
        (char *)list->get(list, 1) == dataAtIndex1);

    mu_assert("The index doesn't point to the right data",
        (char *)list->get(list, 2) == dataAtIndex2);

    return 0;
}

static char *test_foreach()
{
    const char *data[] = {"foo", "bar", "baz"};
    CD9List *list = cd9list_createList();

    list->append(list, (void *)data[0]);
    list->append(list, (void *)data[1]);
    list->append(list, (void *)data[2]);
    
    return 0;

}

static char *test_remove()
{
    const char *data[] = {"foo", "bar", "baz", "bax", "bay"};    
    int status;
    
    CD9List *list = cd9list_createList();

    for(int i = 0; i < 5; i++) {
        list->_insertCopy(list, list->length, (void *)data[i], SIZE_ZERO);
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

    CD9Node *node = cd9list_createNode((void *)data, 4);
    void *copy = cd9list_copyNodeData(node);

    cd9list_deleteNode(node);

    mu_assert("[test_copyNodeData] The node data was not copied properly", 
              !strcmp((void *)copy, data));

    free(copy);

    return 0;
}

static char *test_pop()
{
    const char *data = "foo";

    CD9List *list = cd9list_createList();

    list->append(list, (void *)data);
    list->_insertCopy(list, list->length, (void *)data, 4);

    void *ptrToCopy = list->pop(list);
    void *ptrToData = list->pop(list);

    mu_assert("[test_pop] The size of the list was not set properly", 
              list->length == 0);

    mu_assert("[test_pop] The copy and the string foo are not equal", 
              !strcmp(ptrToCopy, data));

    mu_assert("[test_pop] Pop returned a pointer to the wrong thing", 
              ptrToData == data);
    
    return 0;
}

static char *all_tests() {
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
