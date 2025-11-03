# Initializes the CPU and stack, then jumps to the main entry point
init:
    # initialize stack
    ld r30 12284
    j main

.include ./io.S

main:
    ld r2 150
    call print_dec
done:
    ld r30 36000