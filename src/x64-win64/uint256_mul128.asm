	bits 64
	global mg_uint256_mul128
	
section .text

mg_uint256_mul128:
	push rdi

	mov rdi, rcx
	mov rcx, rdx
; op1: rdi
; op2: rcx

	mov rax, [rdi]
	mul qword [rcx]
	mov r8,  rax
	mov r9, rdx 

	mov rax, [rdi+8]
	mul qword [rcx+8]
	mov r10, rax
	mov r11, rdx 

	mov rax, [rdi+8]
	mul qword [rcx]
	add r9, rax
	adc r10, rdx
	adc r11, 0

	mov rax, [rdi]
	mul qword [rcx+8]
	add r9, rax
	adc r10, rdx
	adc r11, 0

	mov [rdi], r8
	mov [rdi+8], r9
	mov [rdi+16], r10
	mov [rdi+24], r11

	pop rdi
	ret
