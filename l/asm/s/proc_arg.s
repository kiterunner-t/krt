	.file	"proc_arg.c"
	.text
.globl varg
	.type	varg, @function
varg:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$32, %esp
	leal	12(%ebp), %eax
	movl	%eax, -20(%ebp)
	movl	$0, -16(%ebp)
	jmp	.L2
.L7:
	movl	-16(%ebp), %eax
	cmpl	$1, %eax
	je	.L5
	cmpl	$2, %eax
	je	.L6
	testl	%eax, %eax
	jne	.L3
.L4:
		.value	0x0b0f
.L5:
	movl	-20(%ebp), %eax
	leal	4(%eax), %edx
	movl	%edx, -20(%ebp)
	movl	(%eax), %eax
	movl	%eax, -8(%ebp)
	jmp	.L3
.L6:
	movl	-20(%ebp), %eax
	leal	4(%eax), %edx
	movl	%edx, -20(%ebp)
	movl	(%eax), %eax
	movl	%eax, -4(%ebp)
.L3:
	addl	$1, -16(%ebp)
.L2:
	movl	-16(%ebp), %eax
	cmpl	8(%ebp), %eax
	jl	.L7
	leave
	ret
	.size	varg, .-varg
.globl te_varg0
	.type	te_varg0, @function
te_varg0:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$20, %esp
	movl	$0, (%esp)
	call	varg
	movl	%eax, -4(%ebp)
	leave
	ret
	.size	te_varg0, .-te_varg0
.globl te_varg1
	.type	te_varg1, @function
te_varg1:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	$99, 4(%esp)
	movl	$1, (%esp)
	call	varg
	movl	%eax, -4(%ebp)
	leave
	ret
	.size	te_varg1, .-te_varg1
.globl te_varg2
	.type	te_varg2, @function
te_varg2:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$28, %esp
	movl	$1, 8(%esp)
	movl	$99, 4(%esp)
	movl	$2, (%esp)
	call	varg
	movl	%eax, -4(%ebp)
	leave
	ret
	.size	te_varg2, .-te_varg2
.globl te_varg3
	.type	te_varg3, @function
te_varg3:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$32, %esp
	movl	$2, -8(%ebp)
	leal	-8(%ebp), %eax
	movl	%eax, 12(%esp)
	movl	$1, 8(%esp)
	movl	$99, 4(%esp)
	movl	$3, (%esp)
	call	varg
	movl	%eax, -4(%ebp)
	leave
	ret
	.size	te_varg3, .-te_varg3
	.ident	"GCC: (Debian 4.4.5-8) 4.4.5"
	.section	.note.GNU-stack,"",@progbits
