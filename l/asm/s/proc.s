	.file	"proc.c"
	.text
.globl callee
	.type	callee, @function
callee:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -8(%ebp)
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -4(%ebp)
	movl	8(%ebp), %eax
	movl	-4(%ebp), %edx
	movl	%edx, (%eax)
	movl	12(%ebp), %eax
	movl	-8(%ebp), %edx
	movl	%edx, (%eax)
	movl	-4(%ebp), %eax
	movl	-8(%ebp), %edx
	leal	(%edx,%eax), %eax
	leave
	ret
	.size	callee, .-callee
.globl caller
	.type	caller, @function
caller:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	$534, -12(%ebp)
	movl	$1057, -16(%ebp)
	leal	-16(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	-12(%ebp), %eax
	movl	%eax, (%esp)
	call	callee
	movl	%eax, -8(%ebp)
	movl	-12(%ebp), %edx
	movl	-16(%ebp), %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	movl	%eax, -4(%ebp)
	movl	-8(%ebp), %eax
	imull	-4(%ebp), %eax
	leave
	ret
	.size	caller, .-caller
	.ident	"GCC: (Debian 4.4.5-8) 4.4.5"
	.section	.note.GNU-stack,"",@progbits
