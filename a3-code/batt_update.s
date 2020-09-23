	.file	"batt_update.c"
	.text
	.globl	batt_update
	.type	batt_update, @function
batt_update:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	-12(%rbp), %rax
	movq	%rax, %rdi
	call	set_batt_from_ports
	movl	%eax, -16(%rbp)
	cmpl	$0, -16(%rbp)
	jne	.L2
	movl	-12(%rbp), %eax
	leaq	BATT_DISPLAY_PORT(%rip), %rsi
	movl	%eax, %edi
	call	set_display_from_batt
	movl	%eax, -16(%rbp)
.L2:
	movl	-16(%rbp), %eax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L4
	call	__stack_chk_fail@PLT
.L4:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	batt_update, .-batt_update
	.globl	set_batt_from_ports
	.type	set_batt_from_ports, @function
set_batt_from_ports:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movzwl	BATT_VOLTAGE_PORT(%rip), %eax
	testw	%ax, %ax
	jns	.L6
	movl	$1, %eax
	jmp	.L7
.L6:
	movzbl	BATT_STATUS_PORT(%rip), %eax
	movzbl	%al, %eax
	andl	$1, %eax
	testl	%eax, %eax
	jne	.L8
	movq	-8(%rbp), %rax
	movb	$0, 3(%rax)
	jmp	.L9
.L8:
	movzbl	BATT_STATUS_PORT(%rip), %eax
	movzbl	%al, %eax
	andl	$1, %eax
	testl	%eax, %eax
	je	.L9
	movq	-8(%rbp), %rax
	movb	$1, 3(%rax)
.L9:
	movzwl	BATT_VOLTAGE_PORT(%rip), %edx
	movq	-8(%rbp), %rax
	movw	%dx, (%rax)
	movzwl	BATT_VOLTAGE_PORT(%rip), %eax
	cmpw	$2999, %ax
	jg	.L10
	movq	-8(%rbp), %rax
	movb	$0, 2(%rax)
	jmp	.L11
.L10:
	movzwl	BATT_VOLTAGE_PORT(%rip), %eax
	cmpw	$3800, %ax
	jle	.L12
	movq	-8(%rbp), %rax
	movb	$100, 2(%rax)
	jmp	.L11
.L12:
	movzwl	BATT_VOLTAGE_PORT(%rip), %eax
	cwtl
	subl	$3000, %eax
	leal	7(%rax), %edx
	testl	%eax, %eax
	cmovs	%edx, %eax
	sarl	$3, %eax
	movl	%eax, %edx
	movq	-8(%rbp), %rax
	movb	%dl, 2(%rax)
.L11:
	movl	$0, %eax
.L7:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	set_batt_from_ports, .-set_batt_from_ports
	.globl	set_display_from_batt
	.type	set_display_from_batt, @function
set_display_from_batt:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$96, %rsp
	movl	%edi, -84(%rbp)
	movq	%rsi, -96(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$63, -48(%rbp)
	movl	$3, -44(%rbp)
	movl	$109, -40(%rbp)
	movl	$103, -36(%rbp)
	movl	$83, -32(%rbp)
	movl	$118, -28(%rbp)
	movl	$126, -24(%rbp)
	movl	$35, -20(%rbp)
	movl	$127, -16(%rbp)
	movl	$119, -12(%rbp)
	movq	-96(%rbp), %rax
	movl	$0, (%rax)
	movzbl	-81(%rbp), %eax
	testb	%al, %al
	jne	.L14
	movq	-96(%rbp), %rax
	movl	(%rax), %eax
	orl	$2097152, %eax
	movl	%eax, %edx
	movq	-96(%rbp), %rax
	movl	%edx, (%rax)
	movq	-96(%rbp), %rax
	movl	(%rax), %eax
	orl	$4194304, %eax
	movl	%eax, %edx
	movq	-96(%rbp), %rax
	movl	%edx, (%rax)
	movzwl	-84(%rbp), %eax
	addl	$5, %eax
	movw	%ax, -84(%rbp)
	movzwl	-84(%rbp), %eax
	movswl	%ax, %edx
	imull	$-31981, %edx, %edx
	shrl	$16, %edx
	addl	%eax, %edx
	sarw	$9, %dx
	sarw	$15, %ax
	subl	%eax, %edx
	movl	%edx, %eax
	cwtl
	movl	%eax, -60(%rbp)
	movzwl	-84(%rbp), %eax
	movswl	%ax, %edx
	movl	-60(%rbp), %eax
	imull	$-1000, %eax, %eax
	leal	(%rdx,%rax), %ecx
	movl	$1374389535, %edx
	movl	%ecx, %eax
	imull	%edx
	sarl	$5, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, -56(%rbp)
	movzwl	-84(%rbp), %eax
	movswl	%ax, %edx
	movl	-56(%rbp), %eax
	imull	$-100, %eax, %eax
	addl	%eax, %edx
	movl	-60(%rbp), %eax
	imull	$-1000, %eax, %eax
	leal	(%rdx,%rax), %ecx
	movl	$1717986919, %edx
	movl	%ecx, %eax
	imull	%edx
	sarl	$2, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, -52(%rbp)
	movq	-96(%rbp), %rax
	movl	(%rax), %edx
	movl	-60(%rbp), %eax
	cltq
	movl	-48(%rbp,%rax,4), %eax
	sall	$14, %eax
	orl	%eax, %edx
	movq	-96(%rbp), %rax
	movl	%edx, (%rax)
	movq	-96(%rbp), %rax
	movl	(%rax), %edx
	movl	-56(%rbp), %eax
	cltq
	movl	-48(%rbp,%rax,4), %eax
	sall	$7, %eax
	orl	%eax, %edx
	movq	-96(%rbp), %rax
	movl	%edx, (%rax)
	movq	-96(%rbp), %rax
	movl	(%rax), %edx
	movl	-52(%rbp), %eax
	cltq
	movl	-48(%rbp,%rax,4), %eax
	orl	%eax, %edx
	movq	-96(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L15
.L14:
	movzbl	-81(%rbp), %eax
	cmpb	$1, %al
	jne	.L15
	movq	-96(%rbp), %rax
	movl	(%rax), %eax
	orl	$8388608, %eax
	movl	%eax, %edx
	movq	-96(%rbp), %rax
	movl	%edx, (%rax)
	movzbl	-82(%rbp), %ecx
	movsbw	%cl, %dx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$3, %eax
	addl	%edx, %eax
	shrw	$8, %ax
	movl	%eax, %edx
	sarb	$4, %dl
	movl	%ecx, %eax
	sarb	$7, %al
	movl	%edx, %ecx
	subl	%eax, %ecx
	movsbw	%cl, %ax
	imull	$103, %eax, %eax
	shrw	$8, %ax
	movl	%eax, %edx
	sarb	$2, %dl
	movl	%ecx, %eax
	sarb	$7, %al
	subl	%eax, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	subl	%eax, %ecx
	movl	%ecx, %edx
	movsbl	%dl, %eax
	movl	%eax, -72(%rbp)
	movzbl	-82(%rbp), %eax
	movsbw	%al, %dx
	imull	$103, %edx, %edx
	shrw	$8, %dx
	sarb	$2, %dl
	sarb	$7, %al
	movl	%edx, %ecx
	subl	%eax, %ecx
	movsbw	%cl, %ax
	imull	$103, %eax, %eax
	shrw	$8, %ax
	movl	%eax, %edx
	sarb	$2, %dl
	movl	%ecx, %eax
	sarb	$7, %al
	subl	%eax, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	subl	%eax, %ecx
	movl	%ecx, %edx
	movsbl	%dl, %eax
	movl	%eax, -68(%rbp)
	movzbl	-82(%rbp), %ecx
	movsbw	%cl, %ax
	imull	$103, %eax, %eax
	shrw	$8, %ax
	movl	%eax, %edx
	sarb	$2, %dl
	movl	%ecx, %eax
	sarb	$7, %al
	subl	%eax, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	subl	%eax, %ecx
	movl	%ecx, %edx
	movsbl	%dl, %eax
	movl	%eax, -64(%rbp)
	movq	-96(%rbp), %rax
	movl	(%rax), %edx
	movl	-64(%rbp), %eax
	cltq
	movl	-48(%rbp,%rax,4), %eax
	orl	%eax, %edx
	movq	-96(%rbp), %rax
	movl	%edx, (%rax)
	cmpl	$0, -68(%rbp)
	jne	.L16
	cmpl	$0, -72(%rbp)
	je	.L17
.L16:
	movq	-96(%rbp), %rax
	movl	(%rax), %edx
	movl	-68(%rbp), %eax
	cltq
	movl	-48(%rbp,%rax,4), %eax
	sall	$7, %eax
	orl	%eax, %edx
	movq	-96(%rbp), %rax
	movl	%edx, (%rax)
.L17:
	cmpl	$0, -72(%rbp)
	je	.L15
	movq	-96(%rbp), %rax
	movl	(%rax), %edx
	movl	-72(%rbp), %eax
	cltq
	movl	-48(%rbp,%rax,4), %eax
	sall	$14, %eax
	orl	%eax, %edx
	movq	-96(%rbp), %rax
	movl	%edx, (%rax)
.L15:
	movzbl	-82(%rbp), %eax
	cmpb	$4, %al
	jle	.L18
	movq	-96(%rbp), %rax
	movl	(%rax), %eax
	orl	$268435456, %eax
	movl	%eax, %edx
	movq	-96(%rbp), %rax
	movl	%edx, (%rax)
.L18:
	movzbl	-82(%rbp), %eax
	cmpb	$29, %al
	jle	.L19
	movq	-96(%rbp), %rax
	movl	(%rax), %eax
	orl	$134217728, %eax
	movl	%eax, %edx
	movq	-96(%rbp), %rax
	movl	%edx, (%rax)
.L19:
	movzbl	-82(%rbp), %eax
	cmpb	$49, %al
	jle	.L20
	movq	-96(%rbp), %rax
	movl	(%rax), %eax
	orl	$67108864, %eax
	movl	%eax, %edx
	movq	-96(%rbp), %rax
	movl	%edx, (%rax)
.L20:
	movzbl	-82(%rbp), %eax
	cmpb	$69, %al
	jle	.L21
	movq	-96(%rbp), %rax
	movl	(%rax), %eax
	orl	$33554432, %eax
	movl	%eax, %edx
	movq	-96(%rbp), %rax
	movl	%edx, (%rax)
.L21:
	movzbl	-82(%rbp), %eax
	cmpb	$89, %al
	jle	.L22
	movq	-96(%rbp), %rax
	movl	(%rax), %eax
	orl	$16777216, %eax
	movl	%eax, %edx
	movq	-96(%rbp), %rax
	movl	%edx, (%rax)
.L22:
	movl	$0, %eax
	movq	-8(%rbp), %rsi
	xorq	%fs:40, %rsi
	je	.L24
	call	__stack_chk_fail@PLT
.L24:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	set_display_from_batt, .-set_display_from_batt
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
