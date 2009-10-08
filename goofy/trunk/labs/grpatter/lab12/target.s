	.file	"target.c"
	.section	.rodata
	.align	2
.LC0:
	.ascii	"Hijacked !\012\000"
	.text
	.align	2
	.global	never_called
	.type	never_called, %function
never_called:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
.L2:
	ldr	r0, .L4
	bl	puts
	b	.L2
.L5:
	.align	2
.L4:
	.word	.LC0
	.size	never_called, .-never_called
	.align	2
	.global	target_procedure
	.type	target_procedure, %function
target_procedure:
	@ args = 0, pretend = 0, frame = 12
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	sub	sp, sp, #12
	str	r0, [fp, #-24]
	sub	r3, fp, #20
	mov	r0, r3
	bl	gets
	mov	r3, r0
	mov	r0, r3
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
	.size	target_procedure, .-target_procedure
	.section	.rodata
	.align	2
.LC1:
	.ascii	"Welcome to Goofy Giggles\012\000"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	sub	sp, sp, #64
	ldr	r0, .L10
	bl	puts
	sub	r3, fp, #76
	mov	r0, r3
	bl	target_procedure
	mov	r3, #1
	mov	r0, r3
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L11:
	.align	2
.L10:
	.word	.LC1
	.size	main, .-main
	.ident	"GCC: (GNU) 4.0.2"
