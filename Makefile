CC=zig cc
CFLAGS= -Iinclude $(CEXTS)

OUTPUT=main
ifeq ($(OS),Windows_NT)
    OUTPUT = target/asm.exe
endif

SRCS= src/main.c \
      src/util.c \
      src/handlers2.c \
      src/gens2.c \
      src/stage2.c \
	  src/label.c \
	  src/stage1.c \
	  src/handlers1.c \


.PHONY: all clean main target test interrupt


all: clean $(OUTPUT)
test: CEXTS = -DEXT_INT


interrupt: CEXTS = -DEXT_INT
interrupt: all

target/asm.exe: main
	@cp target/asm target/asm.exe
	@echo "Created exe file"

main: target/asm

target:
	@mkdir -p target

target/asm: target $(SRCS)
	@$(CC) $(CFLAGS) -o $@ $(SRCS)
	@echo "Compiled: $@ from $(SRCS)"

test: target/asm test.S
	@./target/asm test.S
	@if [ $$? -eq 0 ]; then \
        echo "Test Successful!"; \
    else \
        echo "Test Failed!"; \
        	exit 1; \
    fi
	@rm a.out

clean:
	@rm -rf target a.out
	@echo "Cleaned all files!"

