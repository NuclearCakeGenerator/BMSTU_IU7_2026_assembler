EXTERN buff_size:byte	
	
	SSTACK SEGMENT para STACK 'STACK'
	db 100 dup(0)
	SSTACK ENDS
	
	
	CSEG SEGMENT para private 'CODE'
assume CS:CSEG, SS:SSTACK
EXTRN calculator:FAR
main:
	mov ax, seg buff_size
	mov ds, ax
	mov ah, 0Ah
	mov dx, offset buff_size
	int 21h
	
	jmp FAR PTR calculator
	CSEG ENDS
	END main
