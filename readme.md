Description
===========

   This is a basic single linked list library that provides all the 
functionalities that you will ever need on a list. You can also use this 
library to simulate stacks and queues. The philosofy behind this project is
that the user should not deal with the internals of it. Thus the interface is
a very easy one to use. 

You should be aware of the fact that this library uses the `C99` standard.

Building the library
====================

    In order to build the library on your local machine, you have to clone the 
project first. After that go in to the directory where the library resides and 
run the following commands:
```
make
sudo make install
make clean
```
The commands above will copy the library into the `/usr/lib/` and the header
files of the library in the directory `/usr/include/cd9/`.

Running tests
=============

   In order to run the tests you must have the most recent version of the 
library installed(if you want the tests to be meaningful). After that run the
following command:
```
make check
```
Tutorial
========

1. Creating and deleting lists
------------------------------

    The function that you will use in order to create a list is 
`cd9list_createList`. After you have done working with the list you will delete
the list using the function `cd9list_deleteList`. The following snippet 
ilutrates the concepts:

```
// creating_and_deleting_lists.c
#include <cd9/cd9list.h>

int main()
{
    // This is how you initialize a list.
    CD9List *list = cd9list_createList();

    // This is how you destroy a list
    cd9list_deleteList(list);

    return 0;
}

```

To compile the source abouve run the following command:
```
gcc -Wall -std=c99 creating_and_deleting_lists.c -lcd9list
```

2. Adding elements
------------------
    
    The 2 main functions used for adding elements to an existing list are 
`append` and `prepend`. There are also some other functions, but they work
similarly. You will pass to both of this functions a pointer to the value
you want to store in the list. Take a look at the following snippet:

```
// append_prepend_and_get.c
#include <stdio.h>
#include <cd9/cd9list.h>

int main()
{
    CD9List *list = cd9list_createList();
            
    // prepend adds an element at the end of the list.
    list->append(list, "cat");
    list->append(list, "dog");
    list->append(list, "fish");

    // So far, the list should look like this: ["cat", "dog", "fish"]
    // Let's print the elements.
                                        
    for(int i = 0; i < 3; i++) {
        // Since the liste stores pointers to the elemnts as void pointers,
        // it is necessary to typecast them before using it.
        printf("%s ", (char *)list->get(list, i));
    }
    printf("\n");
                                                                                  
    // Now we will prepend an item.
    list->prepend(list, "bunny");
    printf("%s \n", (char *)list->get(list, 0));
    
    // This is how we get the last element in the list.
    printf("%s \n", (char *)list->get(list, list->length - 1));
    cd9list_deleteList(list);
    
    return 0;
}

```
To compile the above source file run the following command:
```
gcc -Wall append_prepend_and_get.c -std=c99 -lcd9list

```

3. CD9FOREACH loop
------------------

    The list provides a loop that will make your life easier while dealing with
the datastructures in the library. Take a look at the following example that
prints the elements in a list and then prints the elemnts in a reversed copy
of the list(we used the copy functions just to ilustrate how functions similar
to it works):

```
// foreach_example.c
#include <stdio.h>
#include <cd9/cd9list.h>

int main()
{
    CD9List *list = cd9list_createList();

    // Add some values to the list.
    list->append(list, "car");
    list->append(list, "pen");
    list->append(list, "laptop");

    // The simplest form of the CD9FOREACH loop will pass you at every 
    // iteration each value in the list. It is usefull when you want to iterate
    // over a ist.
    CD9FOREACH(list, value) {
        printf("%s ", (char *)value);
    }
    printf("\n");

    // The copy function returns a copy of the list.
    // Be careful to delete the returned value.
    CD9List *reversed = list->copy(list);

    // Now we eill reverse the copy.
    reversed->reverse(reversed);
    // The second version of CD9FOREACH will pass the valu and the index of 
    // that value at each iteration. This process will be repeated at every
    // iteration.
    CD9FOREACH(reversed, value, index) {
        printf("at index %zu we find %s\n", index, (char *)value);
    }
    
    cd9list_deleteList(reversed);
    cd9list_deleteList(list);

    return 0;
}

```
To compile the source above run:
```
gcc -Wall -std=c99 foreach_example.c  -lcd9list
```

4. *Copy functions
------------------

    You maybe have noticed that there are some functions thet have the `Copy`
suffix. In order to understand them it is very important to understand that
when you append/prepend/insert something into a list you actually add the
address of that value, not the value itself. There are no problems if the
lifetime of the list is no longer than the lifetime of that value. But, when
the lifetime of a value that you added to the list is over the reference 
stored in the list will point to an irelevant value. Functions from *Copy
family will make a copy of the value you want to append and will store in the
list the address of that copy, thus you do not have to care about the lifetime
of the values anymore.














