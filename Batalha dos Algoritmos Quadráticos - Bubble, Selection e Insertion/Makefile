CC ?= gcc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CPPFLAGS ?= -Iinclude
BUILD := build
SRC := src/sorting.c

.PHONY: all clean test
all: $(BUILD)/benchmark

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/benchmark: $(SRC) src/main.c include/sorting.h | $(BUILD)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(SRC) src/main.c -o $@

$(BUILD)/test_sorting: $(SRC) tests/test_sorting.c include/sorting.h | $(BUILD)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(SRC) tests/test_sorting.c -o $@

test: $(BUILD)/test_sorting
	./$(BUILD)/test_sorting

clean:
	rm -rf $(BUILD)
