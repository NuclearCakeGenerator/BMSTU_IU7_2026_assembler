.intel_syntax noprefix
.text
.globl _asm_memmove_copy
.p2align 4, 0x90
_asm_memmove_copy:
    test rdx, rdx
    je done

    mov rcx, rdx
    cmp rdi, rsi
    jb copy_forward

    lea rax, [rsi + rcx]
    cmp rdi, rax
    jae copy_forward

    std
    lea rdi, [rdi + rcx - 1]
    lea rsi, [rsi + rcx - 1]
    rep movsb
    cld
    ret

copy_forward:
    cld
    rep movsb

done:
    ret
