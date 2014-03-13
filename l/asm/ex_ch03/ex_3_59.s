	.file	"ex_3_59.c"
	.text
.globl switch_prob
	.type	switch_prob, @function
switch_prob:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	8(%ebp), %eax
	movl	%eax, -4(%ebp)
	movl	12(%ebp), %eax
	cmpl	$42, %eax
	je	.L3
	cmpl	$42, %eax
	jg	.L6
	cmpl	$40, %eax
	je	.L3
	jmp	.L2
.L6:
	cmpl	$43, %eax
	je	.L4
	cmpl	$44, %eax
	je	.L5
	jmp	.L2
.L3:
	sall	$3, -4(%ebp)
	jmp	.L7
.L4:
	sarl	$3, -4(%ebp)
	jmp	.L7
.L5:
	movl	8(%ebp), %eax
	imull	8(%ebp), %eax
	imull	$49, %eax, %eax
	addl	$17, %eax
	movl	%eax, -4(%ebp)
	jmp	.L7
.L2:
	addl	$17, -4(%ebp)
.L7:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	switch_prob, .-switch_prob
	.ident	"GCC: (Debian 4.4.5-8) 4.4.5"
	.section	.note.GNU-stack,"",@progbits
