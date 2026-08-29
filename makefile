CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

TARGET = shellforge

SRC = src/teach.c \
      src/lexer.c \
      src/token.c \
      src/parser.c \
      src/expand.c \
      src/builtin.c \
      src/execute.c

OBJ = $(SRC:.c=.o)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) -lreadline

clean:
	rm -f $(OBJ) $(TARGET)
