	bits 64
	global mg_uint256_mul_digits

section .data
	JmpTable:	dq	_MUL_0x0, _MUL_1x1, _MUL_2xN, _MUL_3xN, _MUL_4xN, 0, 0, 0

section .text

; int mg_uint256_mul_digits();
; RETURN
;    unoverflow: 0, overflow: 1

; PARAMETERS
;%define OP1			rdi		; const mg_uint256 *
;%define OP1_DIGITS		rsi		; int
;%define OP2			rdx		; const mg_uint256 *
;%define OP2_DIGITS		rcx		; int
;%define RET_VALUE_		r8		; mg_uint256 *

; LOCAL
%define BUFFER			rsp			; uint64_t[9]
%define RET_VALUE		rsp+72		; mg_uint256 *

%define STACK_SIZE		80

mg_uint256_mul_digits:
	push		rbp
	mov			rbp, rsp
	
	; ëÂÇ´Ç¢ï˚Çì‡ë§ÉãÅ[ÉvÇ÷
	cmp			rsi, rcx
	jae			_MUL_BODY
	xchg		rdi, rdx
	xchg		rsi, rcx
_MUL_BODY:
	and			rsi, 0x7
	lea			rax, [REL JmpTable]
	jmp			[rax+rsi*8]

;----------------------------------------------------------------
; multiply 0 * N
;----------------------------------------------------------------
_MUL_0x0:
	pxor	xmm0, xmm0
	movdqu	[r8], xmm0
	movdqu	[r8+16], xmm0

	xor		rax, rax

	mov		rsp, rbp
	pop		rbp
	ret

;----------------------------------------------------------------
; multiply 1 * N
;----------------------------------------------------------------
_MUL_1x1:
	; op1[0]
	mov		rax, [rdi]
	; op1[0] * op2[0]
	mul		qword [rdx]

	pxor	xmm0, xmm0
	mov		[r8], rax
	mov		[r8+8], rdx
	movdqu	[r8+16], xmm0
	
	xor		rax, rax

	mov		rsp, rbp
	pop		rbp
	ret

;----------------------------------------------------------------
; multiply 2 * N
;----------------------------------------------------------------
_MUL_2xN:
	push	rbx
	push	r12
	push	r13
	push	r14
	push	r15
	
	sub		rsp, STACK_SIZE
	
	pxor	xmm0, xmm0
	movdqu	[BUFFER], xmm0
	movdqu	[BUFFER+16], xmm0
	movdqu	[BUFFER+32], xmm0
	movdqu	[BUFFER+48], xmm0
	
	mov		[RET_VALUE], r8
	mov		r8, rdx
	
	xor		r9, r9
_MUL_2xN_LOOP_OP2:
	xor		rbx, rbx

	; &buf[k]
	lea		r10, [BUFFER+r9*8]

	; op1[0]
	mov		rax, [rdi]
	; op1[0] * op2[j]
	mul		qword [r8+r9*8]

	mov		r11, rax
	add		r11, [r10+0]
	mov		r12, rdx
	adc		r12, [r10+8]
	mov		r13, rbx
	adc		r13, [r10+16]
	setb	bl
	
	; op1[1]
	mov		rax, [rdi+8]
	; op1[1] * op2[j]
	mul		qword [r8+r9*8]
	
	add		r12, rax
	adc		r13, rdx
	mov		r14, rbx
	adc		r14, [r10+24]

	mov		[r10+0], r11
	mov		[r10+8], r12
	mov		[r10+16], r13
	mov		[r10+24], r14

	inc		r9
	cmp		r9, rcx

	jb		_MUL_2xN_LOOP_OP2
	
	jmp		_EXIT

;----------------------------------------------------------------
; multiply 3 * N
;----------------------------------------------------------------
_MUL_3xN:
	push	rbx
	push	r12
	push	r13
	push	r14
	push	r15
	
	sub		rsp, STACK_SIZE
	
	pxor	xmm0, xmm0
	movdqu	[BUFFER], xmm0
	movdqu	[BUFFER+16], xmm0
	movdqu	[BUFFER+32], xmm0
	movdqu	[BUFFER+48], xmm0
	
	mov		[RET_VALUE], r8
	mov		r8, rdx

	xor		r9, r9
_MUL_3xN_LOOP_OP2:
	xor		rbx, rbx

	; &buf[k]
	lea		r10, [BUFFER+r9*8]

	; op1[0]
	mov		rax, [rdi]
	; op1[0] * op2[j]
	mul		qword [r8+r9*8]

	mov		r11, rax
	add		r11, [r10+0]
	mov		r12, rdx
	adc		r12, [r10+8]
	mov		r13, rbx
	adc		r13, [r10+16]
	setb	bl
	
	; op1[1]
	mov		rax, [rdi+8]
	; op1[1] * op2[j]
	mul		qword [r8+r9*8]
	
	add		r12, rax
	adc		r13, rdx
	mov		r14, rbx
	adc		r14, [r10+24]
	setb	bl
	
	; op1[2]
	mov		rax, [rdi+16]
	; op1[2] * op2[j]
	mul		qword [r8+r9*8]
	
	add		r13, rax
	adc		r14, rdx
	mov		r15, rbx
	adc		r15, [r10+32]

	mov		[r10+0], r11
	mov		[r10+8], r12
	mov		[r10+16], r13
	mov		[r10+24], r14
	mov		[r10+32], r15

	inc		r9
	cmp		r9, rcx

	jb		_MUL_3xN_LOOP_OP2
	
	jmp		_EXIT

;----------------------------------------------------------------
; multiply 4 * N
;----------------------------------------------------------------
_MUL_4xN:
	push	rbx
	push	r12
	push	r13
	push	r14
	push	r15
	
	sub		rsp, STACK_SIZE
	
	pxor	xmm0, xmm0
	movdqu	[BUFFER], xmm0
	movdqu	[BUFFER+16], xmm0
	movdqu	[BUFFER+32], xmm0
	movdqu	[BUFFER+48], xmm0

	mov		[RET_VALUE], r8
	mov		r8, rdx
	
	xor		r9, r9
_MUL_4xN_LOOP_OP2:
	xor		rbx, rbx

	; &buf[k]
	lea		r10, [BUFFER+r9*8]

	; op1[0]
	mov		rax, [rdi]

	; op1[0] * op2[j]
	mul		qword [r8+r9*8]

	mov		r11, rax
	add		r11, [r10+0]
	mov		r12, rdx
	adc		r12, [r10+8]
	mov		r13, rbx
	adc		r13, [r10+16]
	setb	bl

	; op1[1]
	mov		rax, [rdi+8]

	; op1[1] * op2[j]
	mul		qword [r8+r9*8]
	
	add		r12, rax
	adc		r13, rdx
	mov		r14, rbx
	adc		r14, [r10+24]
	setb	bl

	; op1[2]
	mov		rax, [rdi+16]

	; op1[2] * op2[j]
	mul		qword [r8+r9*8]
	
	add		r13, rax
	adc		r14, rdx
	mov		r15, rbx
	adc		r15, [r10+32]
	setb	bl

	; op1[3]
	mov		rax, [rdi+24]

	; op1[3] * op2[j]
	mul		qword [r8+r9*8]
	
	add		r14, rax
	adc		r15, rdx
	mov		rsi, rbx
	adc		rsi, [r10+40]

	mov		[r10+0], r11
	mov		[r10+8], r12
	mov		[r10+16], r13
	mov		[r10+24], r14
	mov		[r10+32], r15
	mov		[r10+40], rsi

	inc			r9
	cmp			r9,	rcx

	jb			_MUL_4xN_LOOP_OP2
	
	jmp			_EXIT

;----------------------------------------------------------------
; EXIT
;----------------------------------------------------------------
_EXIT:
	mov			rax, qword [BUFFER+32]
	mov			rbx, qword [BUFFER+40]
	or			rax, qword [BUFFER+48]
	or			rbx, qword [BUFFER+56]
	or			rax, rbx

	jz			_NOT_OVERFLOW;
	
	; return overflow
	mov			rax, 1
	
	jmp			_NOT_OVERFLOW_END;
_NOT_OVERFLOW:
	; return not overflow
	xor			rax, rax

	; copy buffer
	mov			rdi, qword [RET_VALUE]

	movdqu		xmm0, [BUFFER]
	movdqu		xmm1, [BUFFER+16]
	movdqu		[rdi], xmm0
	movdqu		[rdi+16], xmm1
_NOT_OVERFLOW_END:

	add		rsp, STACK_SIZE
	
	pop		r15
	pop		r14
	pop		r13
	pop		r12
	pop		rbx

	mov		rsp, rbp
	pop		rbp
	ret
