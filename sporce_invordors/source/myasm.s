.THUMB				@ turn on thumb
.ALIGN  2			@ align code correctly for GBA
.GLOBL  loosealife		@ name of function goes here



.THUMB_FUNC			@ we are about to declare a thumb function
loosealife	:			@ function start

push { r4-r7, lr }	@ push r4-r7 and link register onto stack. Your function might use these
					@ registers, so we need to preserve the values just in case!
					@ we don't need to worry about r0-r3 as it is assumed they will be regularly messed up anyway
ldr r4, [r0]


sub r4, #1

str r4,[r0]

@ the magic happens here!


	
pop { r4-r7 }		@ pop first 4 values from stack back into r4-r7, and also
pop { r3 }			@ pop the next value from stack (stored value for lr) into some unused register, e.g. r3 -
					@ we cannot overwrite lr so we have to do it via a normal register
bx r3				@ "branch and exchange" (return) back to C, using the previous value for lr stored in r3
@ ==================================
