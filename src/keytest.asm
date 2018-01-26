    ; Set int1 vector to point to KeyboardHandler

    LDI R0,0xF9  ; Int1 vector, keyboards
    LDI R1,KeyboardHandler
    ST  R0,R1

    ; Enable keyboard interrupts
    LDI R5,2

forever:
    LDI R0,forever
    JMP R0

KeyboardHandler:

    LDI R0,0x7f ; where the keypress is stored
    LD  R1,R0   ; Load key value into R1
    PRA R1      ; Print it
    IRET        ; Return from interrupt