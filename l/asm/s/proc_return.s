	.file	"proc_return.c"
	.text
.globl stu_alloc
	.type	stu_alloc, @function
stu_alloc:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$20, %esp
	movl	8(%ebp), %ecx
	movl	16(%ebp), %eax
	movw	%ax, -20(%ebp)
	movl	12(%ebp), %eax
	movl	%eax, -8(%ebp)
	movzwl	-20(%ebp), %eax
	movw	%ax, -4(%ebp)
	movl	-8(%ebp), %eax
	movl	-4(%ebp), %edx
	movl	%eax, (%ecx)
	movl	%edx, 4(%ecx)
	movl	%ecx, %eax
	leave
	ret	$4
	.size	stu_alloc, .-stu_alloc
	.section	.rodata
.LC0:
	.string	"krt"
	.text
.globl te_stu
	.type	te_stu, @function
te_stu:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$28, %esp
	leal	-8(%ebp), %eax
	movl	$1, 8(%esp)
	movl	$.LC0, 4(%esp)
	movl	%eax, (%esp)
	call	stu_alloc
	subl	$4, %esp
	leave
	ret
	.size	te_stu, .-te_stu
.globl ll_alloc
	.type	ll_alloc, @function
ll_alloc:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	$8, -8(%ebp)
	movl	$0, -4(%ebp)
	movl	-8(%ebp), %eax
	movl	-4(%ebp), %edx
	leave
	ret
	.size	ll_alloc, .-ll_alloc
.globl te_longlong
	.type	te_longlong, @function
te_longlong:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	call	ll_alloc
	movl	%eax, -8(%ebp)
	movl	%edx, -4(%ebp)
	leave
	ret
	.size	te_longlong, .-te_longlong
.globl d_alloc
	.type	d_alloc, @function
d_alloc:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	fld1
	fstpl	-8(%ebp)
	fldl	-8(%ebp)
	leave
	ret
	.size	d_alloc, .-d_alloc
.globl te_double
	.type	te_double, @function
te_double:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	call	d_alloc
	fstpl	-8(%ebp)
	leave
	ret
	.size	te_double, .-te_double
	.ident	"GCC: (Debian 4.4.5-8) 4.4.5"
	.section	.note.GNU-stack,"",@progbits
