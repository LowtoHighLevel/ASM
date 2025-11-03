MARG=

.PHONY: all clean run

all: a.out

a.out: example.S math.S io.S
	$(MAKE) -C ..
	../target/asm $(MARG) example.S

run: a.out
	$(MAKE) -C ../../CPU/
	../../CPU/target/emu -r -f ./a.out
clean:
	rm -f a.out