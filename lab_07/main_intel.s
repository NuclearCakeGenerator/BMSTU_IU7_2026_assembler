	.build_version macos, 15, 0	sdk_version 15, 4
	.section	__TEXT,__text,regular,pure_instructions
	.intel_syntax noprefix
	.globl	_main                           ## -- Begin function main
	.p2align	4
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset rbp, -16
	mov	rbp, rsp
	.cfi_def_cfa_register rbp
	sub	rsp, 16
	mov	dword ptr [rbp - 4], 0
	mov	dword ptr [rbp - 8], 0
	## InlineAsm Start


	mov	eax, 5



	## InlineAsm End
	mov	dword ptr [rbp - 8], eax
	mov	esi, dword ptr [rbp - 8]
	mov	rdi, qword ptr [rip + __ZNSt3__14coutE@GOTPCREL]
	call	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEElsEi
	xor	eax, eax
	add	rsp, 16
	pop	rbp
	ret
	.cfi_endproc
                                        ## -- End function
.subsections_via_symbols
