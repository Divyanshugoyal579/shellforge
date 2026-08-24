CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

SRC = src/main.c src/lexer.c src/parser.c src/expand.c src/token.c

shellforge:
	$(CC) $(CFLAGS) $(SRC) -o shellforge

