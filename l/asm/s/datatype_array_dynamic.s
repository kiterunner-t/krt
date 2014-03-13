	.file	"datatype_array_dynamic.c"
	.text
	.globl	var_prod_ele
	.type	var_prod_ele, @function
var_prod_ele:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%esi
	pushl	%ebx
	subl	$16, %esp
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	movl	8(%ebp), %eax
	leal	-1(%eax), %edx
	movl	%edx, -20(%ebp)
	movl	8(%ebp), %edx
	leal	-1(%edx), %ecx
	movl	%ecx, -24(%ebp)
	movl	$0, -16(%ebp)
	movl	$0, -12(%ebp)
	jmp	.L2
.L3:
	movl	%eax, %ebx
	movl	20(%ebp), %ecx
	imull	%ebx, %ecx
	leal	0(,%ecx,4), %ebx
	movl	12(%ebp), %ecx
	addl	%ecx, %ebx
	movl	-12(%ebp), %ecx
	movl	(%ebx,%ecx,4), %ebx
	movl	%edx, %esi
	movl	20(%ebp), %ecx
	imull	%esi, %ecx
	leal	0(,%ecx,4), %esi
	movl	16(%ebp), %ecx
	addl	%ecx, %esi
	movl	-12(%ebp), %ecx
	movl	(%esi,%ecx,4), %ecx
	imull	%ebx, %ecx
	addl	%ecx, -16(%ebp)
	addl	$1, -12(%ebp)
.L2:
	movl	-12(%ebp), %ecx
	cmpl	8(%ebp), %ecx
	jl	.L3
	movl	-16(%ebp), %eax
	addl	$16, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	var_prod_ele, .-var_prod_ele
	.ident	"GCC: (Debian 4.7.2-5) 4.7.2"
	.section	.note.GNU-stack,"",@progbits
