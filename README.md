# Sprint-Challenge--Computer-Architecture

## Binary, Decimal, and Hex

Complete the following problems:

* Convert `11001111` binary

    to hex: CF

    to decimal: 207


* Convert `4C` hex

    to binary: 01001100

    to decimal: 76


* Convert `68` decimal

    to binary: 01000100

    to hex: 44


## Architecture

One paragraph-ish:

* Explain how the CPU provides concurrency:

The Central Processing Unit works in the order of four steps.  The first step is the Control Unit (CU) to fetch the instruction from memory.  The second step is the CU to determines what the instruction is and directs the data needed to be moved from memory to the arithmetic/logic unit (ALU).  The third step is for ALU to execute the arithmetic or logical instruction.  Then the final step is for ALU to store the result in memory.

* Describe assembly language and machine language:

The assembly language (ASM) is a low-level programming language used for a computer.  How it works is the assembly language is converted into executable machine code by a utility program or assembler.  The assembler purpose is to assembly the source code.

The lowest-level programming language is machine language.  Machine language consists of numbers.   Programmers would to use assembly language to work with machine language to accomplish the task on hand.

* Suggest the role that graphics cards play in machine learning:

The role that graphics cards play is to provide more processing power up to 200 times.  Graphic cards also support the computer by performing data computation while the CPU does more of render a webpage, drive word processors and other softwares.

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
00000100 # LDI R0,10
00000000
00001010
00000100 # LDI R1,20
00000001
00010100
00010110 # CMP R0,R1
00000000
00000001
00000100 # LDI R2,TEST1
00000010
00010011
00010011 # JEQ R2
00000010
00000100 # LDI R3,1
00000011
00000001
00000110 # PRN R3
00000011
# TEST1 (19):
00010110 # CMP R0,R1
00000000
00000001
00000100 # LDI R2,TEST2
00000010
00100000
00010100 # JNE R2
00000010
00000100 # LDI R3,2
00000011
00000010
00000110 # PRN R3
00000011
# TEST2 (32):
00000100 # LDI R1,10
00000001
00001010
00010110 # CMP R0,R1
00000000
00000001
00000100 # LDI R2,TEST3
00000010
00110000
00010011 # JEQ R2
00000010
00000100 # LDI R3,3
00000011
00000011
00000110 # PRN R3
00000011
# TEST3 (48):
00010110 # CMP R0,R1
00000000
00000001
00000100 # LDI R2,TEST4
00000010
00111101
00010100 # JNE R2
00000010
00000100 # LDI R3,4
00000011
00000100
00000110 # PRN R3
00000011
# TEST4 (61):
00000100 # LDI R3,5
00000011
00000101
00000110 # PRN R3
00000011
00011011 # HLT
```

