CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinc
SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/main
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/filter.c
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

.PHONY: all clean run debug

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

debug: CFLAGS += -g -O0
debug: clean all
