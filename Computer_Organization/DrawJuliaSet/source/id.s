	.data
str_format_sn:	.asciz	"%s\n"
str_format_dn:	.asciz	"%d\n"
str_format_d:	.asciz	"%d"
str_format_c:	.asciz	"%c"
str_inputID:	.asciz	"*****Input ID*****"
str_endPrint:	.asciz	"*****End Print*****"
str_printError:	.asciz	"Error\n"
str_text_enterID:	.asciz	"**Please Enter Member %d ID:**\n"
str_text_enterCmd:	.asciz	"*****Please Enter Commend*****\n"
str_text_printAll:	.asciz	"*****Print Team Member ID and ID Summation*****\n"
str_text_printSum:	.asciz	"\nID Summation: %d\n"

	.align 2
	.text
	.global Id
Id:
	@ set sp, fp, lr
	stmfd sp!, {fp, lr}
	add fp, sp, #4
	sub sp, sp, #12
	@ store argument
	str r0, [fp, #-8]	@id, id
	@ 		[fp, #-12]	@sum
	@ start
	ldr	r0,	= str_format_sn
	ldr	r1,	= str_inputID
	bl	printf
	@ input id
	mov r4, #0
	str r4, [fp, #-12] @0, sum
Id_input_loop:
	ldr r0, = str_text_enterID
	add r1, r4, #1
	bl	printf
	ldr r0, = str_format_d
	ldr r1, [fp, #-8]
	add r1, r1, r4, lsl #2
	bl	scanf
	ldr r0, [fp, #-12]
	ldr r1, [fp, #-8]
	ldr r1, [r1, r4, lsl #2]
	cmp r1, #0
	addpl r0, r0, r1			@ce_1
	strpl r0, [fp, #-12]		@ce_2
	addpl r4, r4, #1			@ce_3
	ldrmi r0, = str_format_sn	@ce_4
	ldrmi r1, = str_printError	@ce_5
	blmi printf
	cmp	r4,	#3
bne	Id_input_loop
	@ scanf cmd
	ldr r0, = str_text_enterCmd
	bl	printf
Id_scanf:
	ldr r0, = str_format_c
	add r1, fp, #-16
	bl	scanf
	ldrb r2, [fp, #-16]
	cmp	r2, #112
bne Id_scanf @ while( != 'p')
	@ output id
	ldr r0, = str_text_printAll
	bl	printf
	mov r4, #0
Id_output_loop:
	ldr r0, = str_format_dn
	ldr r1, [fp, #-8]			@ldr_1
	ldr r1, [r1, r4, lsl #2]	@ldr_2 & op2_1
	bl	printf
	add r4, r4, #1
	cmp	r4,	#3
bne	Id_output_loop
	ldr r0, = str_text_printSum
	ldr r1, [fp, #-12]
	bl	printf
	@ end
	ldr	r0,	= str_format_sn
	ldr	r1,	= str_endPrint
	bl	printf
	@ return argument
	mov ip, #-12
	ldr r0, [fp, ip]			@ldr_3
	@ restore sp, fp, pc
	sub sp, fp, #4
	ldmfd sp!, {fp, pc}
