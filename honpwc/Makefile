CC ?= cc
CFLAGS ?= -Wall -Wextra -g
CPPFLAGS ?=
LDFLAGS ?=
LDLIBS ?=
CLANG_FORMAT ?= clang-format

BUILD_DIR := build

CHAPTERS := 2 3

CHAPTER2_SRCS := $(wildcard chapter2/*.c)
CHAPTER3_SRCS := $(wildcard chapter3/*.c)

CHAPTER2_BINS := $(patsubst chapter2/%.c,$(BUILD_DIR)/chapter2/%,$(CHAPTER2_SRCS))
CHAPTER3_BINS := $(patsubst chapter3/%.c,$(BUILD_DIR)/chapter3/%,$(CHAPTER3_SRCS))

.PHONY: all clean format help chapter2 chapter3 $(addprefix chapter,$(CHAPTERS))

all: $(if $(CHAPTER),chapter$(CHAPTER),$(addprefix chapter,$(CHAPTERS)))

chapter2: $(CHAPTER2_BINS)
chapter3: $(CHAPTER3_BINS)

$(BUILD_DIR)/chapter2/%: chapter2/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

$(BUILD_DIR)/chapter3/%: chapter3/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	$(RM) -r $(BUILD_DIR)

format:
	find . -type d -name $(BUILD_DIR) -prune -o -name '*.c' -exec $(CLANG_FORMAT) -i {} +

help:
	@printf '%s\n' \
		'Targets:' \
		'  make              Build all chapters' \
		'  make chapter2     Build chapter 2 into build/chapter2/' \
		'  make chapter3     Build chapter 3 into build/chapter3/' \
		'  make CHAPTER=3    Build one chapter by number' \
		'  make format       Format all C files with clang-format' \
		'  make clean        Remove build output' \
		'' \
		'Variables:' \
		'  CC, CFLAGS, CPPFLAGS, LDFLAGS, LDLIBS, CLANG_FORMAT'
