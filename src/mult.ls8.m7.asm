# mult.ls8.m7.asm

LDI R0, INT0
LDI R1, 0xF8  ; INT0 address
ST R1,R0 ; Store IRTO processing address
    ; Set int1 vector to point to KeyboardHandler

LDI R0,0xF9  ; Int1 vector, keyboards
LDI R1,KeyboardHandler
ST  R0,R1
LDI R5,255 ; set mask to enable interupts
LDI R0,8
LDI R1,9
MUL R0,R1 
LDI R3,MultBy3 
CALL R3
LDI R4, 5 // pass arg in R4
LDI R3, DivideBy2AddArg
CALL R3
LDI R1,12
SUB R0,R1
PRN R0
NOP
NOP
NOP
HLT
MultBy3:
LDI R2,3
MUL R0,R2
RET
DivideBy2AddArg:
LDI R2,2
DIV R0,R2
ADD R0,R4
RET
INT0: 
LDI R0, HelloINT
LDI R1,12
LDI R2,PrintStr      ; address of PrintStr
CALL R2 
IRET


KeyboardHandler:

    LDI R0,0xf7 ; where the keypress is stored
    LD  R1,R0   ; Load key value into R1
    PRA R1      ; Print it
    IRET        ; Return from interrupt

; Subroutine: PrintStr
; R0 the address of the string
; R1 the number of bytes to print

PrintStr:

	LDI R2,0            ; SAVE 0 into R2 for later CMP

PrintStrLoop:

	CMP R1,R2           ; Compare R1 to 0 (in R2)
	LDI R3,PrintStrEnd  ; Jump to end if we're done
	JEQ R3         

	LD R3,R0            ; Load R3 from address in R0
	PRA R3              ; Print character

	INC R0              ; Increment pointer to next character
	DEC R1              ; Decrement number of characters

	LDI R3,PrintStrLoop ; Keep processing
	JMP R3

PrintStrEnd:

	RET                 ; Return to caller

HelloINT:
TEXT:
ds Hello, INT!
db 0x0a             ; newline
