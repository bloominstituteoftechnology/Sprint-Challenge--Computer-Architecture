; Prints curve histogram
;
; Expected output:
; *
; **
; ****
; ********
; ****************
; ********************************
; ****************************************************************


LDI R0,Histogram  ; Load R1 with the subroutine address
LDI R1,1 ; star counter
LDI R2,0 ; loop counter
CALL R0 ; 1
CALL R0 ; 2
CALL R0 ; 4
CALL R0 ; 8
CALL R0 ; 16
CALL R0 ; 32
CALL R0 ; 64
HLT

Histogram:
    CMP R2,R1 ; compare loop counter 
    INC R2 ; increment loop counter

    ; if less than print *
    LDI R3,PrintStar
    JLT R3

    ; if equal print \n
    LDI R3,PrintNewLine
    JEQ R3

    ; else double star counter and reset loop counter
    LDI R3,1
    SHL R1,R3
    LDI R2,0 ; set loop counter to 0
    RET

PrintStar:
    LDI R3,42 ; *
    PRA R3
    JMP R0

PrintNewLine:
    LDI R3,10 ; \n
    PRA R3
    JMP R0
