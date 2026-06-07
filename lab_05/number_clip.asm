%include "constants.inc"
%include "../chars.inc"
%include "../linux_sys.inc"

BUFFER_SIZE equ 3 ; 3 for octal digits

global print_so_number

section .data
    octal_digits db '0', '1', '2', '3', '4', '5', '6', '7'
    octal_buffer resb BUFFER_SIZE
    db CHAR_CARRIAGE_RETURN, CHAR_LINE_FEED

section .text
print_so_number:
    push rbx ; preserved reg (for modifiable number)

    ; the number is in rdi in 16 bit signed native form
    ; we need to clip it to 8 bit signed and print in octal form
    mov rax, rdi
    and rax, 0b01111111 ; clip to 8 bits
    ; now rax contains the clipped number, but we need to interpret it as signed
    cmp rdi, 0 ; check if the original number was negative
    jge skip_sign_adjustment
    ; if it was negative, we need to convert the clipped number to its signed representation
    or rax, 0b10000000 ; set the sign bit for the clipped
skip_sign_adjustment:
    ; now rax contains the correctly signed clipped number, we need to print it in oct
    ; to print in octal, we can convert the number to a string representation
    ; since the number is at most 8 bits, it can be represented in at most 3 octal digits (0 to 377 in octal)
    ; we will convert the number to octal by repeatedly dividing by 8 and storing the remainders

    mov rbx, BUFFER_SIZE - 1 ; index for octal_buffer
convert_to_octal:
    cmp al, 0
    jne convert_digits
    dec rbx ; move back to the last digit position
    mov [octal_buffer + rbx],byte '0' ; default to '0' for the case when the number is 0
    jmp end_conversion
convert_digits:
    cmp al, 0
    je end_conversion
    mov cl, 8
    div cl ; divide rax by 8, quotient in al, remainder in ah
    xor r8, r8
    mov r8w , ax ; move the remainder to r8b for indexing
    shr r8, 8 ; shift to get the correct value in r8b
    mov cl, [octal_digits + r8] ; convert remainder to octal character
    dec rbx ; move back to the next digit position
    mov [octal_buffer + rbx], cl ; store the octal digit
    mov ah, 0 ; clear al for the next iteration
    jmp convert_digits

end_conversion:
    ; now octal_buffer + rbx points to the start of the octal string
    mov r8, octal_buffer
    add r8, rbx ; move r8 to the start of the octal string
    mov r9, BUFFER_SIZE + 2 ; length of the octal string
    sub r9, rbx ; adjust length based on how many digits we have
    print_string r8, r9 ; print the octal
    pop rbx ; restore preserved reg
    ret
