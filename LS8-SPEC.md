# LS-8 Microcomputer Spec v3.0

## Registers

8 general-purpose 8-bit numeric registers R0-R7.

* R5 is reserved as the interrupt mask (IM)
* R6 is reserved as the interrupt status (IS)
* R7 is reserved as the stack pointer (SP)

> These registers only hold values between 0-255. After performing math on
> registers in the emulator, bitwise-AND the result with 0xFF (255) to keep the
> register values in that range.


## Internal Registers

* `PC`: Program Counter, address of the currently executing instruction
* `IR`: Instruction Register, contains a copy of the currently executing instruction
* `MAR`: Memory Address Register, holds the memory address we're reading or writing //
* `MDR`: Memory Data Register, holds the value to write or the value just read //
* `FL`: Flags, see below


## Flags

The flags register `FL` holds the current flags status. These flags
can change based on the operands given to the `CMP` opcode.

The register is made up of 8 bits. If a particular bit is set, that flag is "true".

`FL` bits: `00000LGE`

* `L` Less-than: during a `CMP`, set to 1 if registerA is less than registerB,
  zero otherwise.
* `G` Greater-than: during a `CMP`, set to 1 if registerA is greater than
  registerB, zero otherwise.
* `E` Equal: during a `CMP`, set to 1 if registerA is equal to registerB, zero
  otherwise.


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
| F7  Reserved          |
| F6  Reserved          |
| F5  Reserved          |
| F4  Key pressed       |    Holds the most recent key pressed on the keyboard
| F3  Top of Stack      |    Stack grows down
| ...                   |
| 00 Program entry      |    Program loaded upward in memory starting at 0
+-----------------------+
    bottom of RAM
```

## Stack

The SP points at the value at the top of the stack, or at address `F4`
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
3. The `PC` register is pushed on the stack.
4. The `FL` register is pushed on the stack.
5. Registers R0-R6 are pushed on the stack in that order.
6. The address (_vector_ in interrupt terminology) of the appropriate
   handler is looked up from the interrupt vector table.
7. Set the PC is set to the handler address.

While an interrupt is being serviced (between the handler being called
and the `IRET`), further interrupts are disabled.

See `IRET`, below, for returning from an interrupt.


### Interrupt numbers

* 0: Timer interrupt. This interrupt triggers once per second.
* 1: Keyboard interrupt. This interrupt triggers when a key is pressed.
  The value of the key pressed is stored in address `0xF4`.

## Instruction Layout

Meanings of the bits in the first byte of each instruction: `AABCCDDD`

* `AA` Number of operands for this opcode, 0-2
* `B` 1 if this is an ALU operation
* `CC` Category, 0-3
* `DDD` Instruction, 0-7

The number of operands `AA` is useful to know because the total number of bytes in any
instruction is the number of operands + 1 (for the opcode). This
allows you to know how far to advance the `PC` with each instruction.

It might also be useful to check the other bits in an emulator implementation, but
there are other ways to code it that don't do these checks.


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
10101000 00000aaa 00000bbb
```

### AND

`AND registerA registerB`

Bitwise-AND registerA and registerB, then store the result in
registerA.

Machine code:
```
10110011 00000aaa 00000bbb
```

### CALL register

`CALL register`

Calls a subroutine (function) at the address stored in the register.

1. The address of the _next_ instruction that will execute is pushed onto the
   stack.
2. The PC is set to the address stored in the given register.

Machine code:
```
01001000 00000rrr
```

### CMP

`CMP registerA registerB`

Compare the value in two registers.

* If they are equal, set the Equal `E` flag to 1, otherwise set it to 0.

* If registerA is less than registerB, set the Less-than `L` flag to 1,
  otherwise set it to 0.

* If registerA is greater than registerB, set the Greater-than `G` flag
  to 1, otherwise set it to 0.

Machine code:
```
10100000 00000aaa 00000bbb
```

### DEC

`DEC register`

Decrement (subtract 1 from) the value in the given register.

Machine code:
```
01111001 00000rrr
```

### DIV

`DIV registerA registerB`

Divide the value in the first register by the value in the second,
storing the result in registerA.

If the value in the second register is 0, the system should print an
error message and halt.

Machine code:
```
10101011 00000aaa 00000bbb
```

### HLT

`HLT`

Halt the CPU (and exit the emulator).

Machine code:
```
00000001 
```

### INC

`INC register`

Increment (add 1 to) the value in the given register.

Machine code:
```
01111000 00000rrr
```

### INT

`INT register`

Issue the interrupt number stored in the given register.

This will set the _n_th bit in the `IS` register to the value in the given
register.

Machine code:
```
01001010 00000rrr
```

### IRET

`IRET`

Return from an interrupt handler.

The following steps are executed:

1. Registers R6-R0 are popped off the stack in that order.
2. The `FL` register is popped off the stack.
3. The return address is popped off the stack and stored in `PC`.
4. Interrupts are re-enabled

Machine code:
```
00001011
```

### JEQ

`JEQ register`

If `equal` flag is set (true), jump to the address stored in the given register.

Machine code:
```
01010001 00000rrr
```

### JGT

`JGT register`

If `greater-than` flag is set (true), jump to the address stored in the given
register.

Machine code:
```
01010100 00000rrr
```

### JLT

`JLT register`

If `less-than` flag is set (true), jump to the address stored in the given
register.

Machine code:
```
01010011 00000rrr
```

### JMP

`JMP register`

Jump to the address stored in the given register.

Set the `PC` to the address stored in the given register.

Machine code:
```
01010000 00000rrr
```

### JNE

`JNE register`

If `E` flag is clear (false, 0), jump to the address stored in the given
register.

Machine code:
```
01010010 00000rrr
```

### LD

`LD registerA registerB`

Loads registerA with the value at the address stored in registerB.

This opcode reads from memory.

Machine code:
```
10011000 00000aaa 00000bbb
```

### LDI

`LDI register immediate`

Set the value of a register to an integer.

Machine code:
```
10011001 00000rrr iiiiiiii
```

### MOD

`MOD registerA registerB`

Divide the value in the first register by the value in the second,
storing the _remainder_ of the result in registerA.

If the value in the second register is 0, the system should print an
error message and halt.

Machine code:
```
10101100 00000aaa 00000bbb
```

### MUL

`MUL registerA registerB`

Multiply two registers together and store the result in registerA.

Machine code:
```
10101010 00000aaa 00000bbb
```

### NOP

`NOP`

No operation. Do nothing for this instruction.

Machine code:
```
00000000
```

### NOT

`NOT register`

Perform a bitwise-NOT on the value in a register.

Machine code:
```
01110000 00000rrr
```

### OR

`OR registerA registerB`

Perform a bitwise-OR between registerA and registerB, storing the
result in registerA.

Machine code:
```
10110001 00000aaa 00000bbb
```

### POP

`POP register`

Pop the value at the top of the stack into the given register.

1. Copy the value from the address pointed to by `SP` to the given register.
2. Increment `SP`.

Machine code:
```
01001100 00000rrr
```

### PRA

`PRA register` pseudo-instruction

Print alpha character value stored in the given register.

Print to the console the ASCII character corresponding to the value in the
register.

Machine code:
```
01000010 00000rrr
```

### PRN

`PRN register` pseudo-instruction

Print numeric value stored in the given register.

Print to the console the decimal integer value that is stored in the given
register.

Machine code:
```
01000011 00000rrr
```

### PUSH

`PUSH register`

Push the given register on the stack.

1. Decrement the `SP`.
2. Copy the value in the given register to the address pointed to by
   `SP`.

Machine code:
```
01001101 00000rrr
```

### RET

`RET`

Return from subroutine.

Pop the value from the top of the stack and store it in the `PC`.

Machine Code:
```
00001001
```

### ST

`ST registerA registerB`

Store value in registerB in the address stored in registerA.

This opcode writes to memory.

Machine code:
```
10011010 00000aaa 00000bbb
```

### SUB

`SUB registerA registerB`

Subtract the value in the second register from the first, storing the
result in registerA.

Machine code:
```
10101001 00000aaa 00000bbb
```

### XOR

`XOR registerA registerB`

Perform a bitwise-XOR between registerA and registerB, storing the
result in registerA.

Machine code:
```
10110010 00000aaa 00000bbb
```