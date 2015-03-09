#include <stdio.h>
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

static char *test_append()
{
    const char *data = "foo";
    CD9Node *lastNode;

    CD9List *list = cd9list_createList();
    list->append(list, (void *)data);

    // Go to the las node.
    for(CD9Node *node = list->nodes; node != NULL; node = node->next) {
        if(node->next == NULL) {
            // We have found the last node.
            lastNode = node;
        }
    }
   
    mu_assert("The value was not appended properly",
        (char *)lastNode->data == data);
    mu_assert("The length of the list was not set properly",
        list->length == 1);

    return 0;
}

static char *test_prepend()
{
    const char *data = "foo";
    CD9List *list = cd9list_createList();
    list->prepend(list, (void *)data);

    mu_assert("Node was not inserted properly", 
        (char *)list->nodes->data == data);
    mu_assert("The length of the list was not modified properly", 
        list->length == 1);

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

static char *all_tests() {
    mu_run_test(test_createNode);
    mu_run_test(test_createList);
//    mu_run_test(test_append);
//    mu_run_test(test_prepend);
//    mu_run_test(test_get);
    mu_run_test(test_getNode);
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
