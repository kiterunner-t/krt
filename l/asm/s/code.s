	.file	"code.c"
	.text
	.p2align 4,,15
.globl sum
	.type	sum, @function
sum:
.LFB2:
	leal	(%rsi,%rdi), %eax
	addl	%eax, accum(%rip)
	ret
.LFE2:
	.size	sum, .-sum
.globl accum
	.section	.bss
	.align 4
	.type	accum, @object
	.size	accum, 4
accum:
	.zero	4
	.section	.eh_frame,"a",@progbits
.Lframe1:
	.long	.LECIE1-.LSCIE1
.LSCIE1:
	.long	0x0
	.byte	0x1
	.string	"zR"
	.uleb128 0x1
	.sleb128 -8
	.byte	0x10
	.uleb128 0x1
	.byte	0x3
	.byte	0xc
	.uleb128 0x7
	.uleb128 0x8
	.byte	0x90
	.uleb128 0x1
	.align 8
.LECIE1:
.LSFDE1:
	.long	.LEFDE1-.LASFDE1
.LASFDE1:
	.long	.LASFDE1-.Lframe1
	.long	.LFB2
	.long	.LFE2-.LFB2
	.uleb128 0x0
	.align 8
.LEFDE1:
	.ident	"GCC: (GNU) 4.2.1 20070719  [FreeBSD]"
