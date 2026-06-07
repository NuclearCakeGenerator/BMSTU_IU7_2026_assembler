.intel_syntax noprefix
.text
.globl asm_memmove_copy32
.p2align 4, 0x90
asm_memmove_copy32:
    push ebp
    mov ebp, esp
    push esi
    push edi

    mov edi, [ebp + 8]
    mov esi, [ebp + 12]
    mov ecx, [ebp + 16]

    test ecx, ecx
    je done

    cmp edi, esi
    jb copy_forward

    lea eax, [esi + ecx]
    cmp edi, eax
    jae copy_forward

    std
    lea edi, [edi + ecx - 1]
    lea esi, [esi + ecx - 1]
    rep movsb
    cld
    jmp done

copy_forward:
    cld
    rep movsb

done:
    mov eax, [ebp + 8]
    pop edi
    pop esi
    pop ebp
    ret
