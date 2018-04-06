# Sprint-Challenge--Computer-Architecture

## Binary, Decimal, and Hex

Complete the following problems:

* Convert `11001111` binary

  * `1100` === 12 Decimal && `1111` === 15 Decimal

        12 = C
        15 = F

    to hex: `CF`

  128 64 32 16 8 4 2 1
  V V V V V V V V
  `1 1 0 0 1 1 1 1`
  128 + 64 + 8 + 4 + 2 + 1 = 207
  to decimal: 207

- Convert `4C` hex

  4 = 0100;
  C = 12 = 1100;

  to binary: `01001100`

  128 64 32 16 8 4 2 1
  V V V V V V V V
  `0 1 0 0 1 1 0 0`

  64 + 8 + 4 = 76 Decimal
  to decimal: `76`

* Convert `68` decimal

  `68`

  128 64 32 16 8 4 2 1
  V V V V V V V V
  `0 1 0 0 0 1 0 0`
  to binary:

  `0100` `0100`
  `4` `4`

  to hex: `44`

## Architecture

One paragraph-ish:

* Explain how the CPU provides concurrency:

  * The CPU reads the instructions it's provided and is able to perform the program associated with the action in a order. It is able to be multiprocessing to perform parallel actions and multitask without having to wait for other instructions to finish.

* Describe assembly language and machine language:

  * Assembly language contains the same instructions as a machine language, but the instructions and variables have names instead of being just numbers. Programs written in high-level languages are translated into assembly language or machine language by a compiler. Assembly language is converted into executable machine code by a utility program referred to as an assembler.

* Suggest the role that graphics cards play in machine learning:
  * GPU provides a more powerful and efficient parallel computing. A single GPU might have thousands of cores while a CPU usually has no more than 12 cores. Although GPU cores are slower than CPU cores, they more than make up for that with their large number and faster memory if the operations can be parallelized. Sequential code is still faster on CPUs.

## Coding

Options for submission, whichever is easier for you:

* Copy your source into this repo, or...
* Submit a PR for the Sprint Challenge from the `Computer-Architecture-One` repo
  you've been using all along.

Sprint Challenge:

[See the LS-8 spec for details](https://github.com/LambdaSchool/Computer-Architecture-One/blob/master/LS8-SPEC.md)

Add the `CMP` instruction and `equal` flag to your LS-8.

Add the `JMP` instruction.

Add the `JEQ` and `JNE` instructions.

[Here is some code](sctest.ls8) that exercises the above instructions. It should
print 1, then 4, then 5.

```
# Code to test the Sprint Challenge
#
# Expected output:
# 1
# 4
# 5

10011001 # LDI R0,10
00000000
00001010
10011001 # LDI R1,20
00000001
00010100
10011001 # LDI R2,TEST1
00000010
00010011
10100000 # CMP R0,R1
00000000
00000001
01010001 # JEQ R2        Does not jump because R0 != R1
00000010
10011001 # LDI R3,1
00000011
00000001
01000011 # PRN R3        Prints 1
00000011

# TEST1 (19):
10011001 # LDI R2,TEST2
00000010
00100000
10100000 # CMP R0,R1
00000000
00000001
01010010 # JNE R2        Jumps because R0 != R1
00000010
10011001 # LDI R3,2
00000011
00000010
01000011 # PRN R3        Skipped--does not print
00000011

# TEST2 (32):
10011001 # LDI R1,10
00000001
00001010
10011001 # LDI R2,TEST3
00000010
00110000
10100000 # CMP R0,R1
00000000
00000001
01010001 # JEQ R2        Jumps becuase R0 == R1
00000010
10011001 # LDI R3,3
00000011
00000011
01000011 # PRN R3        Skipped--does not print
00000011

# TEST3 (48):
10011001 # LDI R2,TEST4
00000010
00111101
10100000 # CMP R0,R1
00000000
00000001
01010010 # JNE R2        Does not jump because R0 == R1
00000010
10011001 # LDI R3,4
00000011
00000100
01000011 # PRN R3        Prints 4
00000011

# TEST4 (61):
10011001 # LDI R3,5
00000011
00000101
01000011 # PRN R3        Prints 5
00000011
10011001 # LDI R2,TEST5
00000010
01001001
01010000 # JMP R2        Jumps unconditionally
00000010
01000011 # PRN R3        Skipped-does not print
00000011

# TEST5 (73):
00000001 # HLT
```
