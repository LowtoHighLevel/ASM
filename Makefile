CC=zig cc
CFLAGS=-Iinclude

SRCS= src/main.c \


.PHONY: all clean main target

all: clean main target/asm

main: target target/asm

target:
	@mkdir -p target

target/asm: $(SRCS)
	@$(CC) $(CFLAGS) -o $@ $(SRCS)
	@echo "Compiled: $@ from $(SRCS)"
clean:
	@rm -rf target
	@echo "Cleaned all files!"

