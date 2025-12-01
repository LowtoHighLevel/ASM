CC=clang
CFLAGS= -Iinclude $(CEXTS)

ZRCROOT ?= /home/cborek/git/zrc/result
ZRC ?= $(ZRCROOT)/bin/zrc
ZRCLIB ?= $(ZRCROOT)/include
ZRFLAGS ?= -I$(ZRCLIB) -I./include $(ZREXTS)
OUTPUT=main
ifeq ($(OS),Windows_NT)
    OUTPUT = target/asm.exe
endif

OUTDIR ?= ./target

ZR_SOURCES ?= $(wildcard src/*.zr)
ZR_OUTPUTS ?= $(ZR_SOURCES:src/%.zr=$(OUTDIR)/%.o)
C_SOURCES ?= $(wildcard src/*.c)
C_OUTPUTS ?= $(C_SOURCES:src/%.c=$(OUTDIR)/%.o)

.PHONY: all clean main test interrupt


all: clean $(OUTPUT)
test: CEXTS = -DEXT_INT


interrupt: CEXTS = -DEXT_INT
interrupt: all

$(OUTDIR)/asm.exe: main
	@cp $(OUTDIR)/asm $(OUTDIR)/asm.exe
	@echo "Created exe file"

main: target $(OUTDIR)/asm

target:
	@mkdir -p $(OUTDIR)

$(OUTDIR)/%.o: src/%.c
	@$(CC) $(CFLAGS) -o $@ -c $<
	@echo "Compiled $@ from $<"

$(OUTDIR)/%.o: src/%.zr
	$(ZRC) --emit object $(ZRFLAGS) -o $@ $<
	@echo "Compiled $@ from $<"

$(OUTDIR)/asm: target $(C_OUTPUTS) $(ZR_OUTPUTS)
	@$(CC) $(CFLAGS) -o $@ $(C_OUTPUTS) $(ZR_OUTPUTS)
	@echo "Compiled: $@ from $(C_OUTPUTS) $(ZR_OUTPUTS)"

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

