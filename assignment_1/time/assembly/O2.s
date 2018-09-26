	.file	"test_time.c"
	.text
	.p2align 4,,15
	.globl	timespec_diff
	.type	timespec_diff, @function
timespec_diff:
.LFB22:
	.cfi_startproc
	movq	(%rsi), %rcx
	movq	8(%rsi), %rax
	subq	(%rdi), %rcx
	subq	8(%rdi), %rax
	js	.L5
	movq	%rcx, (%rdx)
	movq	%rax, 8(%rdx)
	ret
	.p2align 4,,10
	.p2align 3
.L5:
	subq	$1, %rcx
	addq	$1000000000, %rax
	movq	%rcx, (%rdx)
	movq	%rax, 8(%rdx)
	ret
	.cfi_endproc
.LFE22:
	.size	timespec_diff, .-timespec_diff
	.p2align 4,,15
	.globl	sum_numbers
	.type	sum_numbers, @function
sum_numbers:
.LFB23:
	.cfi_startproc
	testq	%rdi, %rdi
	jle	.L9
	addq	$1, %rdi
	movl	$1, %edx
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L8:
	addq	%rdx, %rax
	addq	$1, %rdx
	cmpq	%rdi, %rdx
	jne	.L8
	ret
	.p2align 4,,10
	.p2align 3
.L9:
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE23:
	.size	sum_numbers, .-sum_numbers
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Sum: %li\n"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC1:
	.string	"Time differance:\nSeconds: %d\nNano-seconds: %d\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB24:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$1, %esi
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$56, %rsp
	.cfi_def_cfa_offset 80
	movq	%fs:40, %rax
	movq	%rax, 40(%rsp)
	xorl	%eax, %eax
	movq	%rsp, %rdi
	call	timespec_get@PLT
	leaq	16(%rsp), %rdi
	movl	$1, %esi
	call	timespec_get@PLT
	movq	24(%rsp), %rdx
	movq	16(%rsp), %rbp
	subq	(%rsp), %rbp
	movq	%rdx, %rbx
	subq	8(%rsp), %rbx
	jns	.L13
	subq	$1, %rbp
	addq	$1000000000, %rbx
.L13:
	movabsq	$500000000500000000, %rsi
	leaq	.LC0(%rip), %rdi
	xorl	%eax, %eax
	call	printf@PLT
	xorl	%eax, %eax
	movq	%rbx, %rdx
	movq	%rbp, %rsi
	leaq	.LC1(%rip), %rdi
	call	printf@PLT
	xorl	%eax, %eax
	movq	40(%rsp), %rcx
	xorq	%fs:40, %rcx
	jne	.L16
	addq	$56, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L16:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE24:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.2.1 20180831"
	.section	.note.GNU-stack,"",@progbits
