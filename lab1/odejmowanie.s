.code32
SYSWRITE = 4
STDOUT = 1
SYSEXIT = 1
EXIT_SUCCESS = 0
SYSCALL = 0x80

liczba1:
    .long 0xFFFFFFF0, 0x0000000A, 0x00000005, 0x00000001
liczba2:
    .long 0xFFFFFFFF, 0x00000009, 0x00000004, 0x00000004

.text



.global _start
_start:

clc
movl $4, %edx
pushf
loop_dod:
subl $1, %edx
movl liczba1(,%edx,4), %eax
movl liczba2(,%edx,4), %ebx
popf
sbbl %ebx, %eax
push %eax
pushf

cmp $0, %edx 
jne loop_dod

popf

jnc skok
push $0xFFFFFFF

skok:
push $0



# x/40x $sp - komenda do wyswietlania stosu

mov $SYSEXIT, %eax
mov $EXIT_SUCCESS, %ebx
int $0x80
