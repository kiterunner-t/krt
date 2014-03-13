	.file	"proc_varg4.c"
	.text
	.type	_varg, @function
_varg:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	leal	8(%ebp), %eax
	addl	$4, %eax
	movl	%eax, -8(%ebp)
	addl	$4, -8(%ebp)
	movl	-8(%ebp), %eax
	subl	$4, %eax
	movzbl	(%eax), %eax
	movb	%al, -2(%ebp)
	addl	$4, -8(%ebp)
	movl	-8(%ebp), %eax
	subl	$4, %eax
	movzbl	(%eax), %eax
	movb	%al, -1(%ebp)
	movl	$0, -8(%ebp)
	leave
	ret
	.size	_varg, .-_varg
	.type	_t_varg, @function
_t_varg:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	subl	$40, %esp
	movl	$2, -16(%ebp)
	movw	$1, -12(%ebp)
	movb	$97, -10(%ebp)
	movb	$98, -9(%ebp)
	movl	-16(%ebp), %ebx
	movswl	-12(%ebp),%ecx
	movsbl	-9(%ebp),%edx
	movsbl	-10(%ebp),%eax
	leal	-16(%ebp), %esi
	movl	%esi, 20(%esp)
	movl	%ebx, 16(%esp)
	movl	%ecx, 12(%esp)
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	movl	$5, (%esp)
	call	_varg
	addl	$40, %esp
	popl	%ebx
	popl	%esi
	popl	%ebp
	ret
	.size	_t_varg, .-_t_varg
	.ident	"GCC: (Debian 4.4.5-8) 4.4.5"
	.section	.note.GNU-stack,"",@progbits
