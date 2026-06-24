	StkSeg SEGMENT PARA STACK 'STACK'
	DB 200h DUP (?)
	StkSeg ENDS
	
	DataS SEGMENT WORD 'DATA'
	MyString DB 'Lorem ipSuM'
	DB 10
	DB '$'
	Counter DW 9
	DataS ENDS
	
	Code SEGMENT WORD 'CODE'
ASSUME CS:Code, DS:DataS
DispMsgOnce:
	push AX
	push DX
	mov DX, OFFSET MyString
	mov AH, 9
	int 21h
	pop DX
	pop AX
	ret
	
MultiCall:
	mov AX, DataS
	mov DS, AX
	mov CX, Counter
MyLoop:
	jcxz Exit
	call DispMsgOnce
	dec CX
	jmp MyLoop
	
Exit:
	mov AX, 4C00h
	int 21h
	Code ENDS
END MultiCall
