SYSEXIT = 1
EXIT_SUCCESS = 0
SYSREAD = 3
STDIN = 0
SYSWRITE = 4
STDOUT = 1


.data
    buf: .ascii "     "
    buf_len = . - buf

    key: .ascii    "abcde"
    key_len = . - key

    holder: .ascii " "

.text
    SYSEXIT = 1
    EXIT_SUCCESS = 0
    SYSREAD = 3
    STDIN = 0
    SYSWRITE = 4
    STDOUT = 1

    msgHello: .ascii "Write text (5): '\n"
    msgHello_len = . - msgHello

    msgNE: .ascii "NOT EQUAL\n"
    msgNE_len = . - msgNE

    msgE: .ascii "EQUAL\n"
    msgE_len = . - msgE

    msgKey: .ascii "Key; "
    msgKey_len = . - msgKey

    nl: .ascii "\n"
    nl_len = . - nl

.global _start
_start:
    #SHOW KEY
    mov $SYSWRITE, %eax
    mov $STDOUT, %ebx
    mov $msgKey, %ecx
    mov $msgKey_len, %edx
    int $0x80

    mov $SYSWRITE, %eax
    mov $STDOUT, %ebx
    mov $key, %ecx
    mov $key_len, %edx
    int $0x80

    mov $SYSWRITE, %eax
    mov $STDOUT, %ebx
    mov $nl, %ecx
    mov $nl_len, %edx
    int $0x80

    #WRITE MSG
    mov $SYSWRITE, %eax
    mov $STDOUT, %ebx
    mov $msgHello, %ecx
    mov $msgHello_len, %edx
    int $0x80

    #READ INPUT
    mov $SYSREAD, %eax
    mov $STDIN, %ebx
    mov $buf, %ecx
    mov $buf_len, %edx
    int $0x80

    xor %esi, %esi
    LOOP_BEG:
        mov buf(,%esi,1), %bl
        mov key(,%esi,1), %cl
        cmp %cl, %bl
        jne NOT_EQUAL
        incl %esi
        cmp $buf_len, %esi
        jb LOOP_BEG

    #CORRECT
    mov $SYSWRITE, %eax
    mov $STDOUT, %ebx
    mov $msgE, %ecx
    mov $msgE_len, %edx
    int $0x80

    EXIT:
        mov $SYSEXIT, %eax
        mov $EXIT_SUCCESS, %ebx
        int $0x80


    NOT_EQUAL:
        mov $SYSWRITE, %eax
        mov $STDOUT, %ebx
        mov $msgNE, %ecx
        mov $msgNE_len, %edx
        int $0x80
        jmp EXIT
