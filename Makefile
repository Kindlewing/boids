CC      ?= clang
CSTD    := c99
TARGET  := boids
BUILD   := build

SRC_DIR := src
INC_DIR := include
VEND_DIR:= third_party

SRCS := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

LDLIBS := -lGL -lX11

CFLAGS := \
  -std=$(CSTD) \
  -Wall -Wextra -Wpedantic \
  -I$(INC_DIR) \
  -MMD -MP

$(BUILD)/$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDLIBS)

$(BUILD)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

.PHONY: debug release clean run

debug: CFLAGS += -g -O0
debug: $(BUILD)/$(TARGET)

release: CFLAGS += -O3 -DNDEBUG
release: $(BUILD)/$(TARGET)

run: $(BUILD)/$(TARGET)
	./$<

clean:
	rm -rf $(BUILD)
