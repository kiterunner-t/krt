	.file	"proc_arg1.c"
	.text
.globl varg1
	.type	varg1, @function
varg1:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	leal	12(%ebp), %eax
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	addl	$4, %eax
	movl	%eax, -4(%ebp)
	leave
	ret
	.size	varg1, .-varg1
	.ident	"GCC: (Debian 4.4.5-8) 4.4.5"
	.section	.note.GNU-stack,"",@progbits
