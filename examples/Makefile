MARG=

.PHONY: all clean

all: a.out

a.out: example.S math.S io.S
	$(MAKE) -C ..
	../target/asm $(MARG) example.S

clean:
	rm -f a.out