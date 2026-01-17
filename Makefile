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

LDLIBS := -lGL -lX11

# Default flags (can be overridden by debug/release)
CFLAGS := \
  -std=$(CSTD) \
  -Wall -Wextra -Wpedantic \
  -I$(INC_DIR) \
  -MMD -MP

# -------------------------
# Build targets
# -------------------------

$(BUILD)/$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDLIBS)

$(BUILD)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

# -------------------------
# Debug / Release configurations
# -------------------------

.PHONY: debug release

debug: CFLAGS += -g -O0
debug: $(BUILD)/$(TARGET)

release: CFLAGS += -O3 -DNDEBUG
release: $(BUILD)/$(TARGET)

# -------------------------
# Convenience
# -------------------------

.PHONY: clean run

run: $(BUILD)/$(TARGET)
	./$<

clean:
	rm -rf $(BUILD)
