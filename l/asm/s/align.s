	.file	"align.c"
	.text
.globl slen
	.type	slen, @function
slen:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$32, %esp
	movl	$20, -4(%ebp)
	leal	-32(%ebp), %eax
	movl	%eax, -12(%ebp)
	leal	-32(%ebp), %eax
	addl	$10, %eax
	movl	%eax, -8(%ebp)
	movl	$10, %eax
	leave
	ret
	.size	slen, .-slen
	.ident	"GCC: (Debian 4.4.5-8) 4.4.5"
	.section	.note.GNU-stack,"",@progbits
