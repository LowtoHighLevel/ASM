.include ./math.S

# Prints a character
# r2 - character
print_char:
    write r30 r1
    ld r1 16384
    write r1 r2
    read r30 r1
    ret r26

# prints a number as hex
# r2 - number
print_hex:
    
    # store registers
    write r30 r1
    sub r30 r30 r28

    write r30 r2
    sub r30 r30 r28

    write r30 r3
    sub r30 r30 r28

    write r30 r4
    sub r30 r30 r28

    # temporarily move r2 to print 0x
    mov r4 r2
    ld r2 '0'
    call print_char
    ld r2 'x'
    call print_char
    mov r2 r4

    # print each hex character
    ld r4 4
    ld r3 32
    sub r3 r3 r4
ph_loop:
    call shift_right
    call print_hex_digit
    sub r3 r3 r4
    jz ph_done
    j ph_loop
ph_done:
    call print_hex_digit

    # reload variables and return
    add r30 r30 r28
    read r30 r4
    add r30 r30 r28
    read r30 r3
    add r30 r30 r28
    read r30 r2
    add r30 r30 r28
    read r30 r1

    ret r26


# prints a digit as hex
# r1 - digit
print_hex_digit:

    # store changed registers
    write r30 r1
    sub r30 r30 r28

    write r30 r2
    sub r30 r30 r28

    write r30 r3
    sub r30 r30 r28

    write r30 r4
    sub r30 r30 r28

    # mask the digits to the last 4 bits
    ld r2 15
    and r1 r2 r1

    # check if it is a letter
    ld r2 10
    sub r5 r2 r1
    jn phd_is_letter

    # not a letter
    ld r2 '0'
    add r2 r2 r1
    j phd_do_print

    # is a letter
phd_is_letter:
    sub r1 r1 r2
    ld r2 'A'
    add r2 r2 r1

    # print output
phd_do_print:

    call print_char

    # reload old registers and return
    add r30 r30 r28
    read r30 r4
    add r30 r30 r28
    read r30 r3
    add r30 r30 r28
    read r30 r2
    add r30 r30 r28
    read r30 r1
    ret r26
        
    