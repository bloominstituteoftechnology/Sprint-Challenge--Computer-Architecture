    ; Set int1 vector to point to KeyboardHandler
    LDI RO, 0xF9
    LDI R1,KeyboardHandler
    ST R0,R1

    ; Enable keyboard interrupts
    LDI R5,2

    ; Loop Forever

    LDI R0, Forever
Forever:
 JMP R0

 KeyboardHandler:
 
    LDI R0,0x7f ; where the keypress is stored 
    LD  R1,R0   ; Load key value into R1
    PRA R1      ; Print it
    IRET        ; Return from the interrupt