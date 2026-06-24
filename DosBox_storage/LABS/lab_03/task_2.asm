	public buff_size
	PUBLIC calculator
	
	
	DSEG2 SEGMENT para public 'DATA'
	buff_size db 6
	read_size db ?
	buf db 6 dup(0)
	zero_code db '0'
	DSEG2 ENDS
	
	CSEG SEGMENT para private 'CODE'
assume CS:CSEG, DS:DSEG2
	calculator PROC FAR
	mov ax, DSEG2
	mov ds, ax
	mov dh, read_size
	mov dl, zero_code
	
	mov bl, dh
	mov bh, 0
	sub bx, 2
	mov al, buf[bx]
	sub bx, 1
	mov ah, buf[bx]
	
	sub ah, dl
	sub al, dl
	
	add ah, al
	add ah, dl
	mov dh, ah                   ; Код символа результата
	
	mov ah, 2
	mov dl, 0Dh
	int 21h
	mov ah, 2
	mov dl, 0Ah
	int 21h
	
	mov dl, dh
	mov ah, 2
	int 21h
	
	mov ah, 4ch
	mov al, 0
	int 21h
	
	
	
	calculator ENDP
	CSEG ENDS
	END
