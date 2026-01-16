CC      ?= clang
CSTD    := c99
TARGET  := boids
BUILD   := build

SRC_DIR := src
INC_DIR := include
VEND_DIR:= vendor

SRCS := $(shell find $(SRC_DIR) -type f -name '*.c')

OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

CFLAGS := \
  -std=$(CSTD) \
  -Wall -Wextra -Wpedantic \
  -I$(INC_DIR) \
  -MMD -MP


LDLIBS := -lGL -lX11


$(BUILD)/$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDLIBS)

$(BUILD)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

# -------------------------
# Phony targets
# -------------------------

.PHONY: clean run

run: $(BUILD)/$(TARGET)
	./$<

clean:
	rm -rf $(BUILD)
