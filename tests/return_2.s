    .globl main
main:
    movl $55, %eax
    neg %eax
    not %eax
    cmpl $0, %eax
    movl $0, %eax
    sete %al
    ret
