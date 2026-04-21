CC      = gcc
CFLAGS  = -Wall -Wextra -g
SRC_DIR = src
SRCS    = $(SRC_DIR)/main.c $(SRC_DIR)/graph.c $(SRC_DIR)/union_find.c \
          $(SRC_DIR)/sorting.c $(SRC_DIR)/traversal.c $(SRC_DIR)/dijkstra.c \
          $(SRC_DIR)/mst.c
TARGET  = campus_navigator

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
