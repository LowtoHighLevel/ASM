# Initializes the CPU and stack, then jumps to the main entry point
init:
    # initialize stack
    ld r30 12284
    j main

.include ./io.S

hi_msg:
	.db "HI!"
	.db '\n'
	.db 0

# Main function
main:
    ld r2 255
    call print_hex_fmt
    call print_dec
    ld r2 '\n'
    call print_char
  
    ld r2 2
    ld r3 3
    call mult
    mov r2 r1
    call print_hex_fmt
    
    # Print 'hi' 5 times
    ld r6 5
    ld r5 1
loop1:
    lda r1 hi_msg
    call print_str
    add r1 r1 r5
    call print_str
    # continue the loop until end
    sub r6 r6 r5
    jz done
    j loop1

    # Exit program when done.
done:
    ld r30 36000
