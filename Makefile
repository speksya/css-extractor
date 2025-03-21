CC = gcc
CFLAGS = -Wall -Wextra -I./include -I./external
TARGET_RELEASE = build/release/css-extractor
TARGET_DEBUG = build/debug/css-extractor
INPUT_FILE = input

APP_SOURCES = $(shell find src -name '*.c')
LIB_SOURCES = $(shell find external -name '*.c')

RELEASE_CFLAGS = -O3 -DNDEBUG
DEBUG_CFLAGS = -g -O0 -DDEBUG

RELEASE_OBJECTS = $(patsubst src/%.c,build/release/%.o,$(APP_SOURCES)) \
                  $(patsubst external/%.c,build/release/%.o,$(LIB_SOURCES))

DEBUG_OBJECTS = $(patsubst src/%.c,build/debug/%.o,$(APP_SOURCES)) \
                $(patsubst external/%.c,build/debug/%.o,$(LIB_SOURCES))

.PHONY: all release debug clean run run-debug format lint

all: release

release: $(TARGET_RELEASE)

debug: $(TARGET_DEBUG)

format:
	clang-format -i $(APP_SOURCES) --style=file
	clang-tidy $(APP_SOURCES) -p compile_commands.json --config-file=.clang-tidy

lint:
	cppcheck \
		--enable=all \
		--suppressions-list=cppcheck-suppressions.list \
		--std=c11 \
		--platform=unix64 \
		-I./include \
		-I./external \
		--check-level=exhaustive \
		--verbose \
		$(APP_SOURCES)

run: release
	@if [ -f "$(INPUT_FILE)" ]; then \
		echo "Running RELEASE build with $(INPUT_FILE)"; \
		$(TARGET_RELEASE) "$(INPUT_FILE)"; \
	else \
		echo "Error: input not found in project root"; \
		exit 1; \
	fi

run-debug: debug
	@if [ -f "$(INPUT_FILE)" ]; then \
		echo "Running DEBUG build with $(INPUT_FILE)"; \
		$(TARGET_DEBUG) "$(INPUT_FILE)"; \
	else \
		echo "Error: input not found in project root"; \
		exit 1; \
	fi

$(TARGET_RELEASE): $(RELEASE_OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(RELEASE_CFLAGS) $^ -o $@ $(LDFLAGS)

build/release/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(RELEASE_CFLAGS) -c $< -o $@

build/release/%.o: external/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(RELEASE_CFLAGS) -c $< -o $@

$(TARGET_DEBUG): $(DEBUG_OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEBUG_CFLAGS) $^ -o $@ $(LDFLAGS)

build/debug/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEBUG_CFLAGS) -c $< -o $@

build/debug/%.o: external/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEBUG_CFLAGS) -c $< -o $@

clean:
	rm -rf build/release build/debug

compile-commands:
	@bear -- make clean all
