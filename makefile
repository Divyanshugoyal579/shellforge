CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

SRC = src/main.c src/lexer.c src/token.c src/parser.c src/expand.c

shellforge:
	$(CC) $(CFLAGS) $(SRC) -o shellforge
