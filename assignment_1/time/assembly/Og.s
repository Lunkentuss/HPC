	.file	"test_time.c"
	.text
	.globl	timespec_diff
	.type	timespec_diff, @function
timespec_diff:
.LFB22:
	.cfi_startproc
	movq	8(%rdi), %rax
	cmpq	%rax, 8(%rsi)
	js	.L4
	movq	(%rsi), %rax
	subq	(%rdi), %rax
	movq	%rax, (%rdx)
	movq	8(%rsi), %rax
	subq	8(%rdi), %rax
	movq	%rax, 8(%rdx)
	ret
.L4:
	movq	(%rsi), %rax
	subq	(%rdi), %rax
	subq	$1, %rax
	movq	%rax, (%rdx)
	movq	8(%rsi), %rax
	subq	8(%rdi), %rax
	addq	$1000000000, %rax
	movq	%rax, 8(%rdx)
	ret
	.cfi_endproc
.LFE22:
	.size	timespec_diff, .-timespec_diff
	.globl	sum_numbers
	.type	sum_numbers, @function
sum_numbers:
.LFB23:
	.cfi_startproc
	movl	$1, %edx
	movl	$0, %eax
.L6:
	cmpq	%rdi, %rdx
	jg	.L8
	addq	%rdx, %rax
	addq	$1, %rdx
	jmp	.L6
.L8:
	ret
	.cfi_endproc
.LFE23:
	.size	sum_numbers, .-sum_numbers
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Sum: %ui\n"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC1:
	.string	"Time differance:\nSeconds: %d\nNano-seconds: %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB24:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$72, %rsp
	.cfi_def_cfa_offset 112
	movl	$40, %ebx
	movq	%fs:(%rbx), %rax
	movq	%rax, 56(%rsp)
	xorl	%eax, %eax
	movq	%rsp, %rbp
	movl	$1, %esi
	movq	%rbp, %rdi
	call	timespec_get@PLT
	movl	$400000000, %edi
	call	sum_numbers
	movq	%rax, %r13
	leaq	16(%rsp), %r12
	movl	$1, %esi
	movq	%r12, %rdi
	call	timespec_get@PLT
	leaq	32(%rsp), %rdx
	movq	%r12, %rsi
	movq	%rbp, %rdi
	call	timespec_diff
	movq	%r13, %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	40(%rsp), %rdx
	movq	32(%rsp), %rsi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	56(%rsp), %rcx
	xorq	%fs:(%rbx), %rcx
	jne	.L12
	movl	$0, %eax
	addq	$72, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
.L12:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE24:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.2.1 20180831"
	.section	.note.GNU-stack,"",@progbits
