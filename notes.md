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
* `MAR`: Memory Address Register, holds the memory address we're reading or writing
* `MDR`: Memory Data Register, holds the value to write or the value just read
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
| F3  Start of Stack    |
| F2  [more stack]      |    Stack grows down
| ...                   |
| 01  [more program]    |
| 00  Program entry     |    Program loaded upward in memory starting at 0
+-----------------------+
    bottom of RAM
```

## Stack

The SP points at the value at the top of the stack (most recently pushed), or at
address `F4` if the stack is empty.


### CMP

*This is an instruction handled by the ALU.*

`CMP registerA registerB`

Compare the values in two registers.

* If they are equal, set the Equal `E` flag to 1, otherwise set it to 0.

* If registerA is less than registerB, set the Less-than `L` flag to 1,
  otherwise set it to 0.

* If registerA is greater than registerB, set the Greater-than `G` flag
  to 1, otherwise set it to 0.

Machine code:
```
10100111 00000aaa 00000bbb
A7 0a 0b
```
### HLT

`HLT`

Halt the CPU (and exit the emulator).

Machine code:
```
00000001
01
```
### JEQ

`JEQ register`

If `equal` flag is set (true), jump to the address stored in the given register.

Machine code:
```
01010101 00000rrr
55 0r
```
### JNE

`JNE register`

If `E` flag is clear (false, 0), jump to the address stored in the given
register.

Machine code:
```
01010110 00000rrr
56 0r
```
### LDI

`LDI register immediate`

Set the value of a register to an integer.

Machine code:
```
10000010 00000rrr iiiiiiii
82 0r ii
```

00000111
1111111
