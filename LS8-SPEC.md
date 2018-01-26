# LS-8 Microcomputer Spec v2.0

## Registers

8 general-purpose 8-bit numeric registers R0-R7.

* R5 is reserved as the interrupt mask (IM)
* R6 is reserved as the interrupt status (IS)
* R7 is reserved as the stack pointer (SP)


## Internal Registers

* `PC`: Program Counter, address of the currently executing instruction
* `IR`: Instruction Register, contains a copy of the currently executing instruction
* `MAR`: Memory Address Register, holds the memory address we're reading or writing
* `MDR`: Memory Data Register, holds the value to write or the value just read


## Flags

* `equal`: Flag is set (true) if latest `CMP` was equal


## Memory

The LS-8 has 8-bit addressing, so can address 256 bytes of RAM total.

Memory map:

```
      top of RAM
+-----------------------+
| FF  I7 vector         |    Interrupt vector table
| FE  I6 vector         |
| FD  I5 vector         |
| FC  I4 vector         |
| FB  I3 vector         |
| FA  I2 vector         |
| F9  I1 vector         |
| F8  I0 vector         |
| F7  Top of Stack      |    Stack grows down
|                       |
| ...                   |
|                       |
| 00 Program entry      |    Program loaded upward in memory starting at 0
+-----------------------+
    bottom of RAM
```

## Stack

The SP points at the value at the top of the stack, or at address `F8`
if the stack is empty.


## Interrupts

There are 8 interrupts, I0-I7.

When an interrupt occurs from an external source or from an `INT`
instruction, the appropriate bit in the IS register will be set.

Prior to instruction fetch, the following steps occur:

1. The IM register is bitwise AND-ed with the IS register and the
   results stored as `maskedInterrupts`.
2. Each bit of `maskedInterrupts` is checked, starting from 0 and going
   up to the 7th bit, one for each interrupt.
3. If a bit is found to be set, follow the next sequence of steps. Stop
   further checking of `maskedInterrupts`.

If a bit is set:

1. Disable further interrupts.
2. Clear the bit in the IS register.
3. The PC is pushed on the stack.
4. Registers R0-R7 are pushed on the stack in that order.
5. The address (_vector_ in interrupt terminology) of the appropriate
   handler is looked up from the interrupt vector table.
6. Set the PC is set to the handler address.

While an interrupt is being serviced (between the handler being called
and the `IRET`), further interrupts are disabled.

See `IRET`, below, for returning from an interrupt.


### Interrupt numbers

* 0: Timer interrupt. This interrupt triggers once per second.


## Instruction Set

Glossary:

* **immediate**: takes a constant integer value as an argument
* **register**: takes a register number as an argument

* `iiiiiiii`: 8-bit immediate value
* `00000rrr`: Register number
* `00000aaa`: Register number
* `00000bbb`: Register number

### ADD

`ADD registerA registerB`

Add two registers and store the result in registerA.

Machine code:
```
00001100 00000aaa 00000bbb
```

### CALL register

`CALL register`

Calls a subroutine (function) at the address stored in the register.

Before the call is made, the address of the _next_ instruction that will execute is pushed onto the stack.

Machine code:
```
00001111 00000rrr
```

### CMP

`CMP registerA registerB`

Compare the value in two registers.

If the are equal, set the `equal` flag to true.
If the are not equal, set the `equal` flag to false.

Machine code:
```
00010110 00000aaa 00000bbb
```

### DEC

`DEC register`

Decrement the value in the given register.

Machine code:
```
00011000 00000rrr
```

### DIV

`DIV registerA registerB`

Divide the value in the first register by the value in the second,
storing the result in registerA.

If the value in the second register is 0, the system should print an
error message and halt.

Machine code:
```
0b00001110 00000aaa 00000bbb
```

### HLT

`HLT`

Halt the CPU (and exit the emulator).

Machine code:
```
00011011
```

### INC

`INC register`

Increment the value in the given register.

Machine code:
```
00010111 00000rrr
```

### INT

`INT register`

Issue the interrupt number stored in the given register.

This will set the _n_th bit in the IS register to the value in the given
register.

Machine code:
```
00011001 00000rrr
```

### IRET

`IRET`

Return from an interrupt handler.

The following steps are executed:

1. Registers R7-R0 are popped off the stack in that order.
2. The return address is popped off the stack and stored in PC.
3. Interrupts are re-enabled

Machine code:
```
00011010
```

### JEQ

`JEQ register`

If `equal` flag is set (true), jump to the address stored in the given
register.

Machine code:
```
00010011 00000rrr
```

### JMP

`JMP register`

Jump to the address stored in the given register.

Set the PC to the address stored in the given register.

Machine code:
```
00010001 00000rrr
```

### JNE

`JNE register`

If `equal` flag is clear (false), jump to the address stored in the
given register.

Machine code:
```
00010100 00000rrr
```

### LD

`LD registerA registerB`

Loads registerA with the value at the address stored in registerB.

Machine code:
```
00010010 00000aaa 00000bbb
```

### LDI

`LDI register immediate`

Set the value of a register.

Machine code:
```
00000100 00000rrr iiiiiiii
```
### MUL

`MUL registerA registerB`

Multiply two registers together and store the result in registerA.

Machine code:
```
00000101 00000aaa 00000bbb
```

### NOP

`NOP`

No operation. Do nothing for this instruction.

Machine code:
```
00000000
```

### POP

`POP register`

Pop the value at the top of the stack into the given register.

Machine code:
```
00001011 00000rrr
```

### PRA

`PRA register` pseudo-instruction

Print alpha character value stored in the given register.

Machine code:
```
00000111 00000rrr
```

### PRN

`PRN register` pseudo-instruction

Print numeric value stored in the given register.

Machine code:
```
00000110 00000rrr
```

### PUSH

`PUSH register`

Push the given register on the stack.

Machine code:
```
00001010 00000rrr
```

### RET

`RET`

Return from subroutine.

Pop the value from the top of the stack and store it in the PC.

Machine Code:
```
00010000
```

### ST

`ST registerA registerB`

Store value in registerB in the address stored in registerA.

Machine code:
```
00001001 00000aaa 00000bbb
```

### SUB

`SUB registerA registerB`

Subtract the value in the second register from the first, storing the
result in registerA.

Machine code:
```
00001101 00000aaa 00000bbb
```
