	.file	"datatype_array_2d.c"
	.text
	.globl	array2_prod_ele
	.type	array2_prod_ele, @function
array2_prod_ele:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	$0, -8(%ebp)
	movl	$0, -4(%ebp)
	jmp	.L2
.L3:
	movl	16(%ebp), %eax
	movl	%eax, %edx
	sall	$6, %edx
	movl	8(%ebp), %eax
	addl	%eax, %edx
	movl	-4(%ebp), %eax
	movl	(%edx,%eax,4), %edx
	movl	-4(%ebp), %eax
	movl	%eax, %ecx
	sall	$6, %ecx
	movl	12(%ebp), %eax
	addl	%eax, %ecx
	movl	20(%ebp), %eax
	movl	(%ecx,%eax,4), %eax
	imull	%edx, %eax
	addl	%eax, -8(%ebp)
	addl	$1, -4(%ebp)
.L2:
	cmpl	$15, -4(%ebp)
	jle	.L3
	movl	-8(%ebp), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	array2_prod_ele, .-array2_prod_ele
	.globl	array2_prod_ele_opt
	.type	array2_prod_ele_opt, @function
array2_prod_ele_opt:
.LFB1:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	16(%ebp), %eax
	movl	%eax, %edx
	sall	$6, %edx
	movl	8(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -16(%ebp)
	movl	20(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	12(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -4(%ebp)
	movl	$0, -8(%ebp)
	movl	$0, -12(%ebp)
	jmp	.L6
.L7:
	movl	-12(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	-16(%ebp), %eax
	addl	%edx, %eax
	movl	(%eax), %edx
	movl	-4(%ebp), %eax
	movl	(%eax), %eax
	imull	%edx, %eax
	addl	%eax, -8(%ebp)
	addl	$64, -4(%ebp)
	addl	$1, -12(%ebp)
.L6:
	cmpl	$16, -12(%ebp)
	jne	.L7
	movl	-8(%ebp), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	array2_prod_ele_opt, .-array2_prod_ele_opt
	.globl	array2_set_diag
	.type	array2_set_diag, @function
array2_set_diag:
.LFB2:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	$0, -4(%ebp)
	jmp	.L10
.L11:
	movl	-4(%ebp), %eax
	movl	%eax, %edx
	sall	$6, %edx
	movl	8(%ebp), %eax
	leal	(%edx,%eax), %ecx
	movl	-4(%ebp), %eax
	movl	12(%ebp), %edx
	movl	%edx, (%ecx,%eax,4)
	addl	$1, -4(%ebp)
.L10:
	cmpl	$15, -4(%ebp)
	jle	.L11
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	array2_set_diag, .-array2_set_diag
	.globl	array2_set_diag_opt
	.type	array2_set_diag_opt, @function
array2_set_diag_opt:
.LFB3:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	8(%ebp), %eax
	movl	%eax, -8(%ebp)
	movl	$0, -4(%ebp)
.L13:
	movl	-4(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	-8(%ebp), %eax
	addl	%eax, %edx
	movl	12(%ebp), %eax
	movl	%eax, (%edx)
	addl	$17, -4(%ebp)
	cmpl	$272, -4(%ebp)
	jne	.L13
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	array2_set_diag_opt, .-array2_set_diag_opt
	.ident	"GCC: (Debian 4.7.2-5) 4.7.2"
	.section	.note.GNU-stack,"",@progbits
