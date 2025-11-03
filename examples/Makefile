MARG=

.PHONY: all clean run

all: a.out test.bin

a.out: example.S math.S io.S
	$(MAKE) -C ..
	../target/asm $(MARG) example.S

test.bin: test.S math.S io.S
	$(MAKE) -C ..
	../target/asm $(MARG) test.S test.bin
run: a.out
	$(MAKE) -C ../../CPU/
	../../CPU/target/emu -r -f ./a.out
clean:
	rm -f a.out test.bin