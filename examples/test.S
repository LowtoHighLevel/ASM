# Initializes the CPU and stack, then jumps to the main entry point
init:
    # initialize stack
    ld r30 12284
    j main

test:
	.db 'h'
	.db 'i'
    ld r5 4
    ld r6 2


main:
	lda r2 test
	read r2 r7
	ld r3 1
	ld r4 255

	ld r30 36000
