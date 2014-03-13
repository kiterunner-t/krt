	.file	"datatype_float.c"
	.text
.globl fc
	.type	fc, @function
fc:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	8(%ebp), %eax
	movl	%eax, -24(%ebp)
	movl	12(%ebp), %eax
	movl	%eax, -20(%ebp)
	flds	16(%ebp)
	faddl	-24(%ebp)
	fildl	20(%ebp)
	faddp	%st, %st(1)
	fstpl	-8(%ebp)
	leave
	ret
	.size	fc, .-fc
	.ident	"GCC: (Debian 4.4.5-8) 4.4.5"
	.section	.note.GNU-stack,"",@progbits
