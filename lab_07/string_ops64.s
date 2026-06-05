.intel_syntax noprefix
.text
.globl _asm_memmove_copy64


; rdi = dst
; rsi = src
; rdx = len
_asm_memmove_copy64:
    mov r8, rdi
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
    mov rax, r8
    ret

copy_forward:
    cld
    rep movsb

    mov rax, r8
    ret

done:
    mov rax, r8
    ret
