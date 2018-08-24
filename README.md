# Sprint-Challenge--Computer-Architecture

## Binary, Decimal, and Hex

Complete the following problems:

* Convert `11001111` binary

    11001111 = 1 + 2 + 4 + 8 + 64 + 128 = 207 = 12*16 + 15 = CF

    to hex:

    CF

    to decimal:

    207


* Convert `4C` hex

    4C = 4*16 + 12 = 76 = 64 + 8 + 4 = 01001100

    to binary:

    01001100

    to decimal:

    76


* Convert `68` decimal

    68 = 4*16 + 4 = 0x44 = 0b01000100

    to binary:

    01000100

    to hex:

    44


## Architecture

Short answer:

* Explain how the CPU provides concurrency or parallelism:
  The same way film provides motion: an illusion.  Just as film makes a series of still pictures appear to be a single moving
  picture by showing them in rapid succesion, a CPU appears to run many processes concurrently by rapidly switching between
  them using a scheduling algorithm.

* Describe assembly language and machine language:

  Machine language is the raw numeric language that is actually dealt with by the computer hardware directly;
  it's just numbers between 0 and 255 represented as 8 bits of charge or no charge.

  Assembly is the lowest level of abstraction added for human readability.  It assigns names to the machine language
  instructions.

  sctest.ls8 in this repo is machine language with assembly in the comments.

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


[Here is some code](sctest.ls8) that exercises the above instructions.
It should print:

```
1
4
5
```

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
