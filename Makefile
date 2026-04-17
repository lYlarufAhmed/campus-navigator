CC      = gcc
CFLAGS  = -Wall -Wextra -g
SRC_DIR = src
SRCS    = $(SRC_DIR)/main.c $(SRC_DIR)/graph.c
TARGET  = campus_navigator

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
