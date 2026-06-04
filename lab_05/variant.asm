%include "constants.inc"
%include "../chars.inc"
%include "../linux_sys.inc"

global print_exceeding_degree

section .data
    decade db '1'
    remainder db 0, CHAR_CARRIAGE_RETURN, CHAR_LINE_FEED

section .text
print_exceeding_degree:
    ; the number is in di
    xor rcx, rcx
    xor rax, rax
    mov cx, di
    bsr ax, cx
    inc eax; degree must be greater

    xor edx, edx
    mov ecx, 10
    mov esi , eax ; save the degree in esi for branching
    div ecx ; divide degree by 10 to get the residual
    add dl, '0' ; convert the residual to a character
    mov [remainder], dl ; store the remainder for printing

    lea r8, [decade]
    mov rcx, 4 ; print the decade and the remainder
    cmp esi, 10
    jge skip_adjustment
    dec rcx ; if the degree is less than 10, we only need to print the remainder
    inc r8
skip_adjustment:
    print_string r8, rcx ; print the number
    ret
    