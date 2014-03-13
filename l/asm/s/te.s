	.file	"te.c"
	.text
.globl test
	.type	test, @function
test:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	leal	12(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %eax
	leal	4(%eax), %edx
	movl	%edx, -16(%ebp)
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
	movl	-16(%ebp), %eax
	leal	8(%eax), %edx
	movl	%edx, -16(%ebp)
	fldl	(%eax)
	fstpl	-8(%ebp)
	leave
	ret
	.size	test, .-test
.globl te
	.type	te, @function
te:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	fld1
	fstpl	4(%esp)
	movl	$1, (%esp)
	call	test
	leave
	ret
	.size	te, .-te
	.ident	"GCC: (Debian 4.4.5-8) 4.4.5"
	.section	.note.GNU-stack,"",@progbits
