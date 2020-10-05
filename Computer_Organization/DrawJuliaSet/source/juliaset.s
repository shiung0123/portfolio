	.data
num_1500:	.word 1500
num_4000000:	.word 4000000
	.align 2
	.text
	.global JuliaSet
JuliaSet:
	@ set sp, fp, lr
	stmfd sp!, {fp, lr}
	add fp, sp, #4
	sub sp, sp, #28
	@ store argument
	str r0, [fp, #-8]	@cX, cX
	str r1, [fp, #-12]	@cY, cY
	str r2, [fp, #-16]	@width, width
	str r3, [fp, #-20]	@height, height
	@		[fp, #-24]	@zx
	@		[fp, #-28]	@zy
	@		[fp, #-32]	@color
	@ start
	mov r4, #0			@x
JuliaSet_xfor:
	mov r5, #0			@y
JuliaSet_xfor_yfor:
	@ set zx
    ldr r1, [fp, #-16]	@temp, width
	mov r1, r1, asr #1	@width>>1			@op2_1
	sub r0, r4, r1		@x-(width>>1)		@op2_2
	ldr r2, = num_1500
	ldr r2, [r2]
	mul r0, r0, r2		@1500(x-(width>>1))
	bl	__aeabi_idiv	@1500(x-(width>>1))/(width>>1)
	str r0, [fp, #-24]	@temp, zx
	@ set zy
	ldr r1, [fp, #-20]	@temp, height
	mov r1, r1, asr #1	@height>>1
	sub r0, r5, r1		@y-(height>>1)
	mov r2, #1000
	mul r0, r0, r2		@1500(y-(height>>1))
	bl	__aeabi_idiv	@1500(y-(height>>1))/(height>>1)
	str r0, [fp, #-28]	@temp, zy
	@ set i
	mov r6, #255		@i
JuliaSet_xfor_yfor_while:
	ldr r7, [fp, #-24]	@temp, zx
	mul r0, r7, r7		@zx*zx
	ldr r8, [fp, #-28]	@temp, zy
	mul r1, r8, r8		@zy*zy
	add r2, r0, r1		@zx*zx+zy*zy
	ldr r3, = num_4000000
	ldr r3, [r3]
	cmp r2, r3
	bge while_end
	cmplt r6, #0							@ce_1
	ble while_end	
	sub r0, r0, r1		@zx*zx-zy*zy
	mov r1, #1000
	bl	__aeabi_idiv
	ldr r1, [fp, #-8]	@temp, cX
	add r0, r0, r1
	str r0, [fp, #-24]	@temp, zx
	mul r0, r7, r8		@zx*zy
	mov r1, #500
	bl	__aeabi_idiv
	ldr r1, [fp, #-12]	@temp, cy
	add r0, r0, r1
	str r0, [fp, #-28]	@temp, zy
	sub r6, r6, #1
b	JuliaSet_xfor_yfor_while
while_end:
	@ color value
	and r6, r6, #0xFF						@op2_3
	add r6, r6, r6, lsl #8					@op2_4
	mvn r6, r6
	@ str color
	ldr r0, [fp, #-16]	@temp, width
	mla r0, r0, r5, r4	@w*y+x
	mov r1, #2
	mul r0, r0, r1      @(w*y+x)*2
	ldr r3, [fp, #4]	@temp, frame
	strh r6, [r3, r0]	@frame+(w*y+x)*2
	add r5, r5, #1
	ldr r3, [fp, #-20]	@temp, height
	cmp r5, r3
bne JuliaSet_xfor_yfor
	add r4, r4, #1
	ldr r3, [fp, #-16]	@temp, width
	cmp r4, r3
bne JuliaSet_xfor
	@ restore sp, fp, pc
	@ sub sp, fp, #4
	mov r0, fp
	movcc r1, #3							@ce_2
	movcs r1, #4							@ce_3
	sbcs fp, r0, r1 @ fp=r0-r1+C-1			@sbcs
	mov sp, fp
	@ end sub sp, fp, #4
	ldmfd sp!, {fp, pc}
