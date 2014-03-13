	.file	"ex_3_58.c"
	.text
.globl switch3
	.type	switch3, @function
switch3:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	$0, -4(%ebp)
	cmpl	$4, 16(%ebp)
	ja	.L2
	movl	16(%ebp), %eax
	sall	$2, %eax
	movl	.L8(%eax), %eax
	jmp	*%eax
	.section	.rodata
	.align 4
	.align 4
.L8:
	.long	.L3
	.long	.L4
	.long	.L5
	.long	.L6
	.long	.L7
	.text
.L3:
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -4(%ebp)
	movl	12(%ebp), %eax
	movl	(%eax), %edx
	movl	8(%ebp), %eax
	movl	%edx, (%eax)
	movl	12(%ebp), %eax
	movl	-4(%ebp), %edx
	movl	%edx, (%eax)
	jmp	.L2
.L4:
	movl	12(%ebp), %eax
	movl	(%eax), %edx
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	addl	%eax, %edx
	movl	12(%ebp), %eax
	movl	%edx, (%eax)
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -4(%ebp)
	jmp	.L2
.L5:
	movl	12(%ebp), %eax
	movl	$15, (%eax)
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -4(%ebp)
	jmp	.L2
.L6:
	movl	8(%ebp), %eax
	movl	(%eax), %edx
	movl	12(%ebp), %eax
	movl	%edx, (%eax)
.L7:
	movl	$17, -4(%ebp)
.L2:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	switch3, .-switch3
	.ident	"GCC: (Debian 4.4.5-8) 4.4.5"
	.section	.note.GNU-stack,"",@progbits
