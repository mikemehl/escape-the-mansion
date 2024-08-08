BIN := escape-the-mansion
SRC_DIR := ./src
LIBS_DIR := ./libs
BUILD_DIR := ./build

SRCS := $(shell find $(SRC_DIR) -name '*.c')
SRCS += $(shell find $(LIBS_DIR) -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_DIRS += $(shell find $(LIBS_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

RAYLIB_FLAGS := $(shell pkg-config --cflags --libs raylib)
CFLAGS := -g $(INC_FLAGS) -MMD -MP -Wall -Wextra $(RAYLIB_FLAGS) -std=gnu99
LDFLAGS := $(RAYLIB_FLAGS)

$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c 
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	-rm $(BIN)
	-rm -r $(BUILD_DIR)

-include $(DEPS)
