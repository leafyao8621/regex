CC = gcc
SRC = $(wildcard src/*/*.c) $(wildcard src/*/*/*.c) $(wildcard src/*/*/*/*.c)
OBJ = $(SRC:.c=.o)
LIB = lib/libregex.so
BIN = test
CFLAGS = -Wall -Wextra -Werror -pedantic -fPIC

%.o: %.c
	$(CC) $(CFLAGS) -Iinclude -g -c $< -o $@

$(LIB): $(OBJ)
	$(CC) $(OBJ) -shared -o $(LIB)

$(BIN): $(LIB)
	$(CC) src/test.c -o $(BIN) -Llib -lcontainers -lregex -Iinclude -Wl,-rpath,lib

.PHONY: clean install valgrind
clean:
	@rm $(OBJ)
	@rm $(LIB)
	@rm $(BIN)
valgrind: $(BIN)
	@valgrind -s --leak-check=full --track-origins=yes ./$(BIN)
install:
	@cp -r include/regex ~/.local/include
	@cp $(LIB) ~/.local/lib
uninstall:
	@rm -rf ~/.local/include/regex
	@rm -rf ~/.local/$(LIB)
