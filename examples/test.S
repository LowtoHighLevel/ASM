# Initializes the CPU and stack, then jumps to the main entry point
init:
    # initialize stack
    ld r30 12284
    j main

test_str:
	.db "Hello, World!"

.include ./io.S

main:
	lda r1 test_str
	call print_str
	ld r30 36000
