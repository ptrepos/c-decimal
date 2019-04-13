	bits 64
	global mg_uint256_mul128
	
section .text

mg_uint256_mul128:
	push rdi
	push r12

	mov rdi, rcx
	mov rcx, rdx
; op1: rdi
; op2: rcx
; ret: r8

	mov rax, [rdi]
	mul qword [rcx]
	mov r9,  rax
	mov r10, rdx 

	mov rax, [rdi+8]
	mul qword [rcx+8]
	mov r11, rax
	mov r12, rdx 

	mov rax, [rdi+8]
	mul qword [rcx]
	add r10, rax
	adc r11, rdx
	adc r12, 0

	mov rax, [rdi]
	mul qword [rcx+8]
	add r10, rax
	adc r11, rdx
	adc r12, 0

	mov [r8], r9
	mov [r8+8], r10
	mov [r8+16], r11
	mov [r8+24], r12

	pop r12
	pop rdi
	ret
