CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

SRC = src/main.c src/lexer.c src/token.c src/parser.c src/expand.c src/builtin.c src/executor.c

shellforge:
	$(CC) $(CFLAGS) $(SRC) -o shellforge

clean:
	rm -f shellforge

.PHONY: clean
