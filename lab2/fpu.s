SYSEXIT = 1
EXIT_SUCCESS = 0

.data

nearest:    .word 0x0000
down:       .word 0x0400
up:         .word 0x0800
zero:       .word 0x0C00

single:     .word 0x000
double:     .word 0x200
extened:    .word 0x300

f1: .float -1.999
f2: .float 0.0
f4: .float -9.999
f5: .float -0.0

.text

.global _start
_start:

multiply:
# R7
flds f1
fmuls f2 

# R6
flds f1
fmuls f5

divide:
# R5
flds f1
fdivs f2 

# R4
flds f1
fdivs f5 

# R3
flds f2
fdivs f2

fclex

add:
fldcw extened
# R2
flds f1
fadds f2

sub:
fldcw zero
# R1
flds f1
fsubs f2 

mov $SYSEXIT, %eax
mov $EXIT_SUCCESS, %ebx
int $0x80
