# Initializes the CPU and stack, then jumps to the main entry point
init:
    # initialize stack
    ld r30 12284
    j main



test_str:
	.db "cmd: > "

q_char:
	.db 'q'

new_line:
	.db '\n'
.include ./io.S
main:
	lda r3 q_char
	read r3 r3
	
	lda r4 new_line
	read r4 r4
print_loop:	
	lda r1 test_str
	call print_str

main_loop:
	call read_char

	sub r0 r1 r4
	jz main_loop
	
	sub r0 r1 r3
	jz done
	
	j print_loop
	
done:	
	ld r30 36000
