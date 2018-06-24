.THUMB				@ turn on thumb
.ALIGN  2			@ align code correctly for GBA
.GLOBL  scoreIncrease		@ name of function goes here



.THUMB_FUNC			@ we are about to declare a thumb function
scoreIncrease	:			@ function start

push { r4-r7, lr }	@ push r4-r7 and link register onto stack. Your function might use these
					@ registers, so we need to preserve the values just in case!
					@ we don't need to worry about r0-r3 as it is assumed they will be regularly messed up anyway
	ldr r4, [r0]
	ldr r5, [r1]
	ldr r6, [r2]
	
	mov r7, #36

	cmp r4,r7
	beq EQUAL
	bne NOTEQUAL
	
	
TENEQUAL:
	add r6, #1
	sub r5, #9
	
	
	str r6,[r2] 

EQUAL :
	
	add r5, #1
	sub r4, #9
	
	
	
	cmp r5, r7
	beq TENEQUAL

	
	str r4, [r0]
	str r5, [r1]



	



NOTEQUAL :
	add r4, #1
	str r4,[r0]
	


@ the magic happens here!


	
pop { r4-r7 }		@ pop first 4 values from stack back into r4-r7, and also
pop { r3 }			@ pop the next value from stack (stored value for lr) into some unused register, e.g. r3 -
					@ we cannot overwrite lr so we have to do it via a normal register
bx r3				@ "branch and exchange" (return) back to C, using the previous value for lr stored in r3
@ ==================================
