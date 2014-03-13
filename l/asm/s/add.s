	.file	"add.cc"
	.text
.globl _Z4swapRiS_
	.type	_Z4swapRiS_, @function
_Z4swapRiS_:
.LFB0:
	.cfi_startproc
	.cfi_personality 0x0,__gxx_personality_v0
	pushl	%ebp
	.cfi_def_cfa_offset 8
	movl	%esp, %ebp
	.cfi_offset 5, -8
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	8(%ebp), %eax
	movl	(%eax), %edx
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, -4(%ebp)
	leave
	ret
	.cfi_endproc
.LFE0:
	.size	_Z4swapRiS_, .-_Z4swapRiS_
.globl _Z4testv
	.type	_Z4testv, @function
_Z4testv:
.LFB1:
	.cfi_startproc
	.cfi_personality 0x0,__gxx_personality_v0
	pushl	%ebp
	.cfi_def_cfa_offset 8
	movl	%esp, %ebp
	.cfi_offset 5, -8
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	$2, -4(%ebp)
	movl	$7, -8(%ebp)
	leal	-8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	-4(%ebp), %eax
	movl	%eax, (%esp)
	call	_Z4swapRiS_
	leave
	ret
	.cfi_endproc
.LFE1:
	.size	_Z4testv, .-_Z4testv
	.ident	"GCC: (Debian 4.4.5-8) 4.4.5"
	.section	.note.GNU-stack,"",@progbits
