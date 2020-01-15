				
				
				AREA asm_functions, CODE, READONLY				
                EXPORT  ASM_volume
ASM_volume
				; save current SP for a faster access 
				; to parameters in the stack
				MOV   r12, sp
				; save volatile registers
				STMFD sp!,{r4-r8,r10-r11,lr}	
				
				; R0 = sin value
				; R1 = volume %
				; val (R0) = R0 * R1 / 80; 80 = volume source sin.
				MOV R4, #80
				
				MUL R0, R0, R1
				UDIV R0, R0, R4
				; R0 is the return value

				; restore volatile registers
				LDMFD sp!,{r4-r8,r10-r11,pc}
				
                END