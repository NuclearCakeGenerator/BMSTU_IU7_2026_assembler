.386

OK EQU 0

TICKS_IN_SECOND EQU 5
MAX_SPEED EQU 0
MIN_SPEED EQU 11111b

StkSeg SEGMENT USE16 STACK 'STACK'
    db 100h DUP(?)
StkSeg ENDS

CodeSeg SEGMENT USE16 PUBLIC 'Code'
ASSUME cs:CodeSeg, ds:CodeSeg, es:CodeSeg, ss:StkSeg

tick_counter db 0
speed db MIN_SPEED
old_int1Ch_handler dd ?

my_int_1Ch:
    push ax
    push bx
    push ds
    push es
    push dx

    ; pushf
    ; call far ptr cs:[old_int1Ch_handler]

    mov ax, CodeSeg
    mov ds, ax

    inc tick_counter
    cmp tick_counter, TICKS_IN_SECOND
    jb my_int_1Ch_done

    mov ah, 03h
    mov al, 05h
    mov bh, 0
    mov bl, speed
    int 16h

    ; Обновление скорости
    mov tick_counter, 0

    cmp speed, MAX_SPEED
    je reset_speed

    dec speed
    jmp my_int_1Ch_done

reset_speed:
    mov speed, MIN_SPEED
    
my_int_1Ch_done:
    ; отображение текущей скорости
    mov dl, 10
    xor ah, ah
    mov al, speed
    div dl
    add al, '0'
    add ah, '0'

    mov bx, 0B800h
    mov es, bx
    mov bx, 78 * 2

    mov byte ptr es:[bx], al
    mov byte ptr es:[bx+1], 00100000b
    mov byte ptr es:[bx+2], ah
    mov byte ptr es:[bx+3], 00100000b

    pop  dx
    pop  es
    pop  ds
    pop  bx
    pop  ax
    jmp dword ptr cs:[old_int1Ch_handler]
    ; iret
my_int_1Ch_end:

main:
    mov ax, CodeSeg
    mov ds, ax

    ; -- подмена прерывания таймера --
    ; получени старого адреса прерывания в ES:BX
    mov ah, 35h
    mov al, 1Ch
    int 21h 
    mov word ptr old_int1Ch_handler, bx
    mov word ptr old_int1Ch_handler+2, es



    ; подмена прерывания int 1Ch
    mov dx, offset my_int_1Ch
    mov ah, 25h
    mov al, 1Ch 
    int 21h


    
    ; -- Вывод для проверки, что ничего не зависло -- 
    mov ah, 02h
    mov dl, 'O'
    int 21h

    ; Вычисление количества параграфов
    mov dx, offset my_int_1Ch_end
    add dx, 15
    shr dx, 4

    ; Заверщение резидентно
    mov ah, 31h
    mov al, OK
    int 21h

CodeSeg ENDS
END main
