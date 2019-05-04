	bits 64
	global mg_uint256_mul_digits:function hidden

section .text
	JmpTable:
	  dw _MUL_0x0 - _MUL_BASE
	  dw _MUL_1x1 - _MUL_BASE
	  dw _MUL_2xN - _MUL_BASE
	  dw _MUL_3xN - _MUL_BASE
	  dw _MUL_4xN - _MUL_BASE
	  dw 0,
	  dw 0,
	  dw 0

; int mg_uint256_mul_digits();
; RETURN
;    unoverflow: 0, overflow: 1

; PARAMETERS
%define OP1				rdi			; const mg_uint256 *
%define OP1_DIGITS		rsi			; int
%define OP2				rdx			; const mg_uint256 *
%define OP2_DIGITS		rcx			; int
%define RET_VALUE		r8			; mg_uint256 *

; LOCAL
%define BUFFER			rsp			; unsigned long long[9]

%define STACK_SIZE		72

mg_uint256_mul_digits:
	
	; ëÂÇ´Ç¢ï˚Çì‡ë§ÉãÅ[ÉvÇ÷
	cmp OP1_DIGITS, OP2_DIGITS
	jae _MUL_BODY
	xchg OP1, OP2
	xchg OP1_DIGITS, OP2_DIGITS
_MUL_BODY:
	and OP1_DIGITS, 0x7
	lea rax, [rel JmpTable]
	lea r10, [rel _MUL_BASE]
	movzx rax, word [rax+OP1_DIGITS*2]
	add rax, r10
	jmp rax
	
_MUL_BASE:
;----------------------------------------------------------------
; multiply 0 * N
;----------------------------------------------------------------
_MUL_0x0:
	pxor xmm0, xmm0
	movups [RET_VALUE], xmm0
	movups [RET_VALUE+16], xmm0

	xor rax, rax

	ret

;----------------------------------------------------------------
; multiply 1 * N
;----------------------------------------------------------------
_MUL_1x1:
	; op1[i]
	mov rax, [OP1]
	; op1[i] * op2[j]
	mul qword [OP2]
	
	pxor xmm0, xmm0
	mov [RET_VALUE], rax
	mov [RET_VALUE+8], rdx
	movups [RET_VALUE+16], xmm0
	
	xor rax, rax

	ret

;----------------------------------------------------------------
; multiply 2 * N
;----------------------------------------------------------------
_MUL_2xN:
	push rbp
	mov rbp, rsp
	
	push rbx
	push r12
	push r13
	push r14
	push r15
	
	sub rsp, STACK_SIZE
	
	mov rsi, rdx
	
%define OP1 rdi ; const mg_uint256 *
%define OP2 rsi ; const mg_uint256 *
%define OP2_DIGITS rcx ; int
%define RET_VALUE r8 ; mg_uint256 *
%define BUF_ADDR r9 ; mg_uint256 *

	pxor xmm0, xmm0
	movups [BUFFER], xmm0
	movups [BUFFER+16], xmm0
	movups [BUFFER+32], xmm0
	movups [BUFFER+48], xmm0

	xor r10, r10
	cmp r10, OP2_DIGITS
	jnb _MUL_2xN_LOOP_OP_END
_MUL_2xN_LOOP_OP2:
	xor ebx, ebx

	; &buf[k]
	lea BUF_ADDR, [BUFFER+r10*8]

	; op1[i]
	mov rax, [OP1]

	; op1[i] * op2[j]
	mul qword [OP2+r10*8]

	mov r11, rax
	add r11, [BUF_ADDR+0]
	mov r12, rdx
	adc r12, [BUF_ADDR+8]
	mov r13, 0
	adc r13, [BUF_ADDR+16]
	setc bl
	
	mov rax, [OP1+8]

	; op1[i+1] * op2[j]
	mul qword [OP2+r10*8]
	
	add r12, rax
	adc r13, rdx
	mov r14, rbx
	adc r14, [BUF_ADDR+24]

	mov [BUF_ADDR+0], r11
	mov [BUF_ADDR+8], r12
	mov [BUF_ADDR+16], r13
	mov [BUF_ADDR+24], r14

	inc r10
	cmp r10, OP2_DIGITS
	jb _MUL_2xN_LOOP_OP2
_MUL_2xN_LOOP_OP_END:
	
	jmp _EXIT

;----------------------------------------------------------------
; multiply 3 * N
;----------------------------------------------------------------
_MUL_3xN:
	push rbp
	mov rbp, rsp
	
	push rbx
	push r12
	push r13
	push r14
	push r15
	
	sub rsp, STACK_SIZE
	
	mov rsi, rdx
	
%define OP1 rdi ; const mg_uint256 *
%define OP2 rsi ; const mg_uint256 *
%define OP2_DIGITS rcx ; int
%define RET_VALUE r8 ; mg_uint256 *
%define BUF_ADDR r9 ; mg_uint256 *

	pxor	xmm0,		xmm0
	movups	[BUFFER],		xmm0
	movups	[BUFFER+16],	xmm0
	movups	[BUFFER+32],	xmm0
	movups	[BUFFER+48],	xmm0

	xor r10, r10
	cmp r10, OP2_DIGITS
	jnb _MUL_3xN_LOOP_OP_END
_MUL_3xN_LOOP_OP2:
	xor ebx, ebx

	; &buf[k]
	lea BUF_ADDR, [BUFFER+r10*8]

	; op1[0]
	mov rax, [OP1]

	; op1[0] * op2[j]
	mul qword [OP2+r10*8]

	mov r11, rax
	add r11, [BUF_ADDR+0]
	mov r12, rdx
	adc r12, [BUF_ADDR+8]
	mov r13, rbx
	adc r13, [BUF_ADDR+16]
	setc bl
	
	; op1[1]
	mov rax, [OP1+8]

	; op1[1] * op2[j]
	mul qword [OP2+r10*8]
	
	add r12, rax
	adc r13, rdx
	mov r14, rbx
	adc r14, [BUF_ADDR+24]
	setc bl
	
	; op1[2]
	mov rax, [OP1+16]

	; op1[2] * op2[j]
	mul qword [OP2+r10*8]
	
	add r13, rax
	adc r14, rdx
	mov r15, rbx
	adc r15, [BUF_ADDR+32]

	mov [BUF_ADDR+0], r11
	mov [BUF_ADDR+8], r12
	mov [BUF_ADDR+16], r13
	mov [BUF_ADDR+24], r14
	mov [BUF_ADDR+32], r15

	inc r10
	cmp r10, OP2_DIGITS
	jb _MUL_3xN_LOOP_OP2
_MUL_3xN_LOOP_OP_END:
	
	jmp _EXIT

;----------------------------------------------------------------
; multiply 4 * N
;----------------------------------------------------------------
_MUL_4xN:
	push rbp
	mov rbp, rsp
	
	push	rbx
	push	r12
	push	r13
	push	r14
	push	r15
	
	sub		rsp,	STACK_SIZE
	
	mov rsi, rdx
	
	pxor xmm0, xmm0
	movdqu [BUFFER], xmm0
	movdqu [BUFFER+16], xmm0
	movdqu [BUFFER+32], xmm0
	movdqu [BUFFER+48], xmm0

%define OP1 rdi ; const mg_uint256 *
%define OP2 rsi ; const mg_uint256 *
%define OP2_DIGITS rcx ; int
%define RET_VALUE r8 ; mg_uint256 *
%define BUF_ADDR r9 ; mg_uint256 *

	xor r10, r10
	cmp r10, OP2_DIGITS
	jnb _MUL_4xN_LOOP_OP2_END
_MUL_4xN_LOOP_OP2:
	xor ebx, ebx

	; &buf[k]
	lea BUF_ADDR, [BUFFER+r10*8]

	; op1[0]
	mov rax, [OP1]

	; op1[0] * op2[j]
	mul qword [OP2+r10*8]

	mov r11, rax
	add r11, [BUF_ADDR+0]
	mov r12, rdx
	adc r12, [BUF_ADDR+8]
	mov r13, rbx
	adc r13, [BUF_ADDR+16]
	setc bl

	; op1[1]
	mov rax, [OP1+8]

	; op1[1] * op2[j]
	mul qword [OP2+r10*8]
	
	add r12, rax
	adc r13, rdx
	mov r14, rbx
	adc r14, [BUF_ADDR+24]
	setc bl

	; op1[2]
	mov rax, [OP1+16]

	; op1[2] * op2[j]
	mul qword [OP2+r10*8]
	
	add r13, rax
	adc r14, rdx
	mov r15, rbx
	adc r15, [BUF_ADDR+32]
	setc bl

	; op1[3]
	mov rax, [OP1+24]

	; op1[3] * op2[j]
	mul qword [OP2+r10*8]
	
	add r14, rax
	adc r15, rdx
	adc rbx, [BUF_ADDR+40]

	mov [BUF_ADDR+0], r11
	mov [BUF_ADDR+8], r12
	mov [BUF_ADDR+16], r13
	mov [BUF_ADDR+24], r14
	mov [BUF_ADDR+32], r15
	mov [BUF_ADDR+40], rbx

	inc r10
	cmp r10, OP2_DIGITS
	jb _MUL_4xN_LOOP_OP2
_MUL_4xN_LOOP_OP2_END:
	
	jmp _EXIT

;----------------------------------------------------------------
; EXIT
;----------------------------------------------------------------
_EXIT:
	mov rax, qword [BUFFER+32]
	mov rcx, qword [BUFFER+40]
	or rax, qword [BUFFER+48]
	or rcx, qword [BUFFER+56]
	or rax, rcx

	jz _NOT_OVERFLOW;
	
	; return overflow
	mov rax, 1
	
	jmp _NOT_OVERFLOW_END;
_NOT_OVERFLOW:
	; return not overflow
	xor rax, rax

	; copy buffer
	movups xmm0, [BUFFER]
	movups xmm1, [BUFFER+16]
	movups [RET_VALUE], xmm0
	movups [RET_VALUE+16], xmm1
_NOT_OVERFLOW_END:

	add			rsp, STACK_SIZE
	
	pop			r15
	pop			r14
	pop			r13
	pop			r12
	pop			rbx

	mov			rsp, rbp
	pop			rbp
	ret
