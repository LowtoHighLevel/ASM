.include ./math.S

# Prints a character
# r2 - character
print_char:
    write r30 r1
    ld r1 16384
    write r1 r2
    read r30 r1
    ret r26

# Prints a number as hex (with 0x before and \n after)
#
# r2 - number
print_hex_fmt:
    write r30 r4
    mov r4 r2
    ld r2 '0'
    call print_char
    ld r2 'x'
    call print_char
    mov r2 r4
    call print_hex
    ld r2 '\n'
    call print_char
    mov r2 r4
    read r30 r4
    ret r26

# returns the first hex bit that is not 0
# input: r2
# amount: r3
# return: r3
first_bit_hex:
    write r30 r4
    sub r30 r30 r28

    write r30 r1
    sub r30 r30 r28

    ld r4 15
fbh_loop:
    sub r3 r3 r28
    call shift_right
    and r0 r1 r4
    jz fbh_loop

    add r3 r3 r28
    
    add r30 r30 r28
    read r30 r1

    add r30 r30 r28
    read r30 r4
    ret r26

# prints a number as hex
# r2 - number
print_hex:
    write r30 r3
    ld r3 32
    call first_bit_hex
ph_loop:
    # r28 has a 4 because of method call
    sub r3 r3 r28
    call shift_right
    call print_hex_digit
    add r0 r0 r3
    jz ph_done
    j ph_loop

ph_done:
    read r30 r3
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

    write r30 r5
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
    read r30 r5
    add r30 r30 r28
    read r30 r4
    add r30 r30 r28
    read r30 r3
    add r30 r30 r28
    read r30 r2
    add r30 r30 r28
    read r30 r1
    ret r26
        
    