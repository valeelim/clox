NAME := clox
C_LANG := 

CC := clang
CFLAGS := -g -Wextra -Werror -Wno-unused-parameter

BUILD_DIR := build
SOURCE_DIR := ./

# Files
HEADERS := $(wildcard $(SOURCE_DIR)/*.h)
SOURCES := $(wildcard $(SOURCE_DIR)/*.c)
OBJECTS := $(addprefix $(BUILD_DIR)/$(NAME)/, $(notdir $(SOURCES:.c=.o)))

# Targets ------------------------------------------------------------------

# Link the interpreter
$(NAME): $(OBJECTS)
	@ printf "%8s %-20s %s\n" $(CC) $@ "$(CFLAGS)"
	@ mkdir -p build
	@ $(CC) $(CFLAGS) $^ -o $@

# Compile object files
$(BUILD_DIR)/$(NAME)/%.o: $(SOURCE_DIR)/%.c $(HEADERS)
	@ printf "%8s %-20s %s\n" $(CC) $< "$(CFLAGS)"
	@ mkdir -p $(BUILD_DIR)/$(NAME)
	@ time $(CC) -c $(C_LANG) $(CFLAGS) -o $@ $<

run: $(NAME)
	@ ./$(NAME) lox

clean:
	@ rm -rf build/

.PHONY: default clean run
