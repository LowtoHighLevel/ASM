# Shift Left
# r1 - output
# r2 - input
# r3 - amount
shift_left:
    # push non-changing registers to the stack
    write r30 r4
    sub r30 r30 r28

    write r30 r5
    sub r30 r30 r28

    # set up output
    mov r1 r2

    # base case
    add r0 r0 r3
    jz shr_done

    # set up loop
    mov r5 r3
    ld r4 1
shl_loop:
    # clear the carry bit
    add r0 r0 r0
    # rotate left
    rol r1 r1
    
    # handle loop
    sub r5 r5 r4
    jz shl_done
    j shl_loop
shl_done:
    # pop unused items from the stack   
    add r30 r30 r28
    read r30 r5

    add r30 r30 r28
    read r30 r4
    # return (overwrites r26 with the address)
    ret r26

# Shift Right
# r1 - output
# r2 - input
# r3 - amount
shift_right:
    # push non-changing registers to the stack
    write r30 r4
    sub r30 r30 r28

    write r30 r5
    sub r30 r30 r28

    # set up output
    mov r1 r2

    # base case
    add r0 r0 r3
    jz shr_done

    # set up loop
    mov r5 r3
    ld r4 1
shr_loop:
    # clear the carry bit
    add r0 r0 r0

    # rotate left
    ror r1 r1
    
    # handle loop
    sub r5 r5 r4
    jz shr_done
    j shr_loop
shr_done:
    # pop unused items from the stack   
    add r30 r30 r28
    read r30 r5

    add r30 r30 r28
    read r30 r4
    # return (overwrites r26 with the address)
    ret r26

# Multiplication
#
# r1 - output
# r2 - a input
# r3 - b input
mult:

    # store used variables
    write r30 r4

    # result = 0
    ld r1 0
    # if b == 0, return
    add r0 r0 r3
    jz mult_done
    # while b > 0
    ld r4 1
mult_loop:
    # b & 1
    and r0 r4 r3
    # if b & 1 == 0 skip adding
    jz mult_loop_skip
    # result += a
    add r1 r1 r2
mult_loop_skip:
    # a <<= 1
    add r0 r0 r0
    rol r2 r2
    # b >>= 1
    add r0 r0 r0
    ror r3 r3
    # if (b == 0) we are done
    jz mult_done
    # loop again
    j mult_loop
mult_done:
    
    read r30 r4
    ret r26