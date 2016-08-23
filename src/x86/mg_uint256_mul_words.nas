	global _mg_uint256_mul_words

section .text

;int mg_uint256_mul_words()
;	[ebp+8]		op1				const mg_uint256 *
;	[ebp+12]	op1_digits		int
;	[ebp+16]	op2				const mg_uint256 *
;	[ebp+20]	op2_digits		int
;	[ebp+24]	ret				mg_uint256 *
;
;	return		unoverflow: 0, overflow: 1

_mg_uint256_mul_words:
	push		ebp
	mov			ebp, esp
	
	; [esp]		buf		72byte
	; ecx		j
	; esi		i
	; ebx		carry

	push		ebx
	push		edi
	push		esi

	sub			esp,	72
	
	pxor		xmm1, 		xmm1
	movdqu		[esp],		xmm1
	movdqu		[esp+16],	xmm1
	movdqu		[esp+32],	xmm1
	movdqu		[esp+48],	xmm1

	xor			ecx,	ecx
_LOOP_OP2:
	xor			esi,	esi
	xor			ebx,	ebx

_LOOP_OP1:
	; k = i + j
	lea			edx,	[ecx+esi]
	; &buf[k]
	lea			edi,	[esp+edx*4]

	; op1[i]
	mov			edx,	[ebp+8]

	; op2[j]
	mov			eax,	[ebp+16]
	mov			eax,	[eax+ecx*4]

	; op1[i] * op2[j]
	mul			dword [edx+esi*4]
	
	add			[edi+0],	eax
	adc			[edi+4],	edx
	adc			[edi+8],	ebx
	setb		bl

	inc			esi
	cmp			esi,	[ebp+12]

	jb			_LOOP_OP1;
	
	inc			ecx
	cmp			ecx,	[ebp+20]

	jb			_LOOP_OP2;

	mov			eax,	dword [esp+32]
	mov			ebx,	dword [esp+36]
	or			eax,	dword [esp+40]
	or			ebx,	dword [esp+44]
	or			eax,	dword [esp+48]
	or			ebx,	dword [esp+52]
	or			eax,	dword [esp+56]
	or			ebx,	dword [esp+60]
	or			eax,	ebx

	jz			_NOT_OVERFLOW;
	
	; return overflow
	mov			eax,	1
	
	jmp			_NOT_OVERFLOW_END;
_NOT_OVERFLOW:
	; return not overflow
	xor			eax,	eax

	; copy buffer
	mov			edi,			dword [ebp+24]

	movdqu		xmm1,			[esp]
	movdqu		xmm2,			[esp+16]
	movdqu		[edi+0],		xmm1
	movdqu		[edi+16],		xmm2
_NOT_OVERFLOW_END:

	add			esp,	72

	pop			esi
	pop			edi
	pop			ebx

	mov			esp,	ebp
	pop			ebp
	ret
