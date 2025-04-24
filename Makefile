CC=zig cc
CFLAGS=-Iinclude

OUTPUT=main
ifeq ($(OS),Windows_NT)
    OUTPUT = target/asm.exe
endif

SRCS= src/main.c \
      src/util.c \
      src/handlers2.c \
      src/gens2.c \
      src/stage2.c \


.PHONY: all clean main target

all: clean $(OUTPUT)

target/asm.exe: main
	@cp target/asm target/asm.exe
	@echo "Created exe file"

main: target target/asm

target:
	@mkdir -p target

target/asm: $(SRCS)
	@$(CC) $(CFLAGS) -o $@ $(SRCS)
	@echo "Compiled: $@ from $(SRCS)"
clean:
	@rm -rf target
	@echo "Cleaned all files!"

