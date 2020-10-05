	.data
str_format_sn:	.asciz	"%s\n"
str_printName:	.asciz	"*****Print Name*****"
str_endPrint:	.asciz	"*****End Print*****"

TEAM:
	.asciz	"Team 19"

NAME_15:
	.asciz	"I-Hsiang Hsu"
	.space	2
	.asciz	"Yun-He Wang"
	.space	3
	.asciz	"Yun-He Wang"

	.align 2
	.text
	.global Name
Name:
	@ set sp, fp, lr
	stmfd sp!, {fp, lr}
	add fp, sp, #4
	sub sp, sp, #8
	@ store argument
	str r0, [fp, #-8] @team, team
	str r1, [fp, #-12] @name, name
	@ start
	ldr	r0,	= str_format_sn
	ldr	r1,	= str_printName
	bl	printf
	@ strcpy( team, team )
	ldr r0, [fp, #-8] @team, team
	ldr r1, = TEAM
	mov r2, #15
	bl	memcpy
	ldr r0, = str_format_sn
	bl	printf
	@ strcpy( name, name )
	mov r4, #0
Name_loop:
	ldr r0, [fp, #-12] @name, name
	ldr r1, = NAME_15
	mov r2, #15
	mla	r0,	r2,	r4,	r0
	mla	r1,	r2,	r4,	r1
	bl	memcpy
	ldr r0, = str_format_sn
	bl	printf
	@ add r4, r4, #1
	mov r1, r4
	mov r2, #0
	adcs r0, r1, r2				@ adcs_1
	mov r4, r0
	cmp	r4,	#3
bne	Name_loop
	@ end
	ldr	r0,	= str_format_sn
	ldr	r1,	= str_endPrint
	bl	printf
	@ restore sp, fp, pc
	sub sp, fp, #4
	ldmfd sp!, {fp, pc}
