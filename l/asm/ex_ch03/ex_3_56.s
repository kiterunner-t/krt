	.file	"ex_3_56.c"
	.text
.globl loop
	.type	loop, @function
loop:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$16, %esp
	movl	$1431655765, -12(%ebp)
	movl	$-2147483648, -8(%ebp)
	jmp	.L2
.L3:
	movl	12(%ebp), %eax
	movl	-8(%ebp), %edx
	leal	(%edx,%eax), %eax
	xorl	%eax, -12(%ebp)
	movl	12(%ebp), %eax
	movl	-12(%ebp), %edx
	movl	%edx, %ebx
	movl	%eax, %ecx
	sarl	%cl, %ebx
	movl	%ebx, %eax
	movl	%eax, -8(%ebp)
.L2:
	cmpl	$0, -8(%ebp)
	jne	.L3
	movl	-12(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	popl	%ebp
	ret
	.size	loop, .-loop
	.ident	"GCC: (Debian 4.4.5-8) 4.4.5"
	.section	.note.GNU-stack,"",@progbits
