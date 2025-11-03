# Initializes the CPU and stack, then jumps to the main entry point
init:
    # initialize stack
    ld r30 12284
    j main

.include ./io.S

# prints hi
print_hi:
    write r30 r2
    
    ld r2 'H'
    call print_char

    ld r2 'I'
    call print_char

    ld r2 '!'
    call print_char

    ld r2 '\n'
    call print_char

    read r30 r2
    ret r26

# Main function
main:
    ld r2 255
    call print_hex_fmt
  
    ld r2 2
    ld r3 3
    call mult
    mov r2 r1
    call print_hex_fmt
    
    # Print 'hi' 5 times
    ld r1 5
    ld r5 1
loop1:

    call print_hi
    # continue the loop until end
    sub r1 r1 r5
    jz done
    j loop1

    # Exit program when done.
done:
    ld r30 36000