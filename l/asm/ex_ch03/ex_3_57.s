	.file	"ex_3_57.c"
	.text
.globl cread
	.type	cread, @function
cread:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	$0, -4(%ebp)
	cmpl	$0, 8(%ebp)
	jne	.L2
	leal	-4(%ebp), %eax
	jmp	.L3
.L2:
	movl	8(%ebp), %eax
.L3:
	movl	%eax, 8(%ebp)
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	leave
	ret
	.size	cread, .-cread
	.ident	"GCC: (Debian 4.4.5-8) 4.4.5"
	.section	.note.GNU-stack,"",@progbits
