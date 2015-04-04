CC              = gcc
SOURCES         = ./src/cd9list.c ./src/callbacks.c
CFLAGS          = -Wall -std=c99 -fPIC -c
LIB_OPTIONS     = -shared -o
BINARY_LOCATION = ./bin/libcd9list.so
OBJECT_FILES    = callbacks.o cd9list.o
TEST_FILES      = ./tests/tests_cd9list.c
TEST_FLAGS      = -Wall -std=c99 -lcd9list -o
TEST_BINARY     = ./bin/tests

all:
	$(CC) $(CFLAGS) $(SOURCES) 
	$(CC) $(LIB_OPTIONS) $(BINARY_LOCATION) $(OBJECT_FILES) 
	@echo "Delete the object files"
	@rm -rf $(OBJECT_FILES)

install:
	@echo "Intalling the header files"
	@mkdir /usr/include/cd9/
	@cp ./src/cd9list.h /usr/include/cd9/
	@cp ./src/macro_dispatcher.h /usr/include/cd9/
	@cp ./src/va_numargs.h /usr/include/cd9/
	@cp ./src/callbacks.h /usr/include/cd9/
	@echo "Installing the shared library"
	@mv $(BINARY_LOCATION) /usr/lib/

uninstall:
	@rm -rf /usr/lib/libcd9list.so
	@echo "Removing the shared library"
	@rm -rf /usr/include/cd9
	@echo "Removing the header files"

check:
	$(CC) $(TEST_FILES) $(TEST_FLAGS) $(TEST_BINARY)
	$(TEST_BINARY)

clean:
	@echo "Deleting binaries"
	@rm -rf ./bin/*

