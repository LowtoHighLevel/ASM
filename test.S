ld r1 'H'
ld r2 ' '
ld r3 '\n'
ld r4 16384
ld r5 1
mov r7 r3
add r6 r1 r5
write r4 r1
write r4 r2
write r4 r6
write r4 r3

ld r5 '0'
ld r1 1
add r6 r5 r1
write r4 r6
write r4 r7

ld r2 1

add r6 r5 r2
write r4 r6
write r4 r7

and r3 r2 r1
add r6 r5 r3
write r4 r6
write r4 r7


ld r2 0
add r6 r5 r2
write r4 r6
write r4 r7

or r3 r2 r1
add r6 r5 r3
write r4 r6
write r4 r7

ld r1 1
ror r1 r1

ldflags r3
ld r2 1
ror r3 r3
and r1 r2 r3
add r6 r5 r1
write r4 r6
write r4 r7


ld r1 1
ror r1 r1
cflag 1

ldflags r3
ld r2 1
ror r3 r3
and r1 r2 r3
add r6 r5 r1
write r4 r6
write r4 r7

