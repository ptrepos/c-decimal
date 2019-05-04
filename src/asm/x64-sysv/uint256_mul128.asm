	bits 64
	global mg_uint256_mul128:function hidden
	
section .text

mg_uint256_mul128:
; op1: rdi
; op2: rsi

	mov rax, [rdi]
	mul qword [rsi]
	mov r8, rax
	mov r9, rdx 

	mov rax, [rdi+8]
	mul qword [rsi+8]
	mov r10, rax
	mov r11, rdx 

	mov rax, [rdi+8]
	mul qword [rsi]
	add r9, rax
	adc r10, rdx
	adc r11, 0

	mov rax, [rdi]
	mul qword [rsi+8]
	add r9, rax
	adc r10, rdx
	adc r11, 0

	mov [rdi], r8
	mov [rdi+8], r9
	mov [rdi+16], r10
	mov [rdi+24], r11

	ret
