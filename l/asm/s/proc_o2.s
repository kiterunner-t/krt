	.file	"proc.c"
	.text
	.p2align 4,,15
.globl callee
	.type	callee, @function
callee:
	pushl	%ebp
	movl	%esp, %ebp
	movl	12(%ebp), %ecx
	pushl	%ebx
	movl	8(%ebp), %ebx
	movl	(%ecx), %edx
	movl	(%ebx), %eax
	movl	%edx, (%ebx)
	movl	%eax, (%ecx)
	leal	(%edx,%eax), %eax
	popl	%ebx
	popl	%ebp
	ret
	.size	callee, .-callee
	.p2align 4,,15
.globl caller
	.type	caller, @function
caller:
	pushl	%ebp
	movl	$832093, %eax
	movl	%esp, %ebp
	popl	%ebp
	ret
	.size	caller, .-caller
	.ident	"GCC: (Debian 4.4.5-8) 4.4.5"
	.section	.note.GNU-stack,"",@progbits
