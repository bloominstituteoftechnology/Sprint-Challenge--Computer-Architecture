# Sprint-Challenge--Computer-Architecture

## Binary, Decimal, and Hex

Complete the following problems:

*   Convert `11001111` binary

    to hex: 00CF

    to decimal: 207

-   Convert `4C` hex

    to binary: 1001100

    to decimal: 76

*   Convert `68` decimal

    to binary: 1000100

    to hex: 0044

## Architecture

Short answer:

*   Explain how the CPU provides concurrency or parallelism:

    Concurrency allows CPUs to greatly improve its overall speed of execution of computer programs. CPUs provide concurrency by delegating certain operations to its different components. For example, peripherals read and write data to memory using the DAM. This prevents the CPU from being slowed down by having to process potentially large data files.

*   Describe assembly language and machine language:

    Assembly language is a low-level programming language that translates a higher level language's program into machine code instructions.

    Machine language is a programming language that utilizes instructions, hexadecimal or binary, that is executed by a computer's CPU. Each instructions encodes a specific task and every program the CPU executes is made up of multiple instructions.

*   Why is 3D performance so much higher with a graphics card than
    without?

    Graphics cards are a type of processor that are specialized to handle 2D and 3D graphics. Modern cards are very specialized to the point that they can carry out graphical calculations much more efficiently than a CPU.

*   Suggest the role that graphics cards play in machine learning:

    Computer scientists use GPUs to train neural networks at a larger scale and even faster than multi-core CPUs. GPU libraries allowed computer scientists and developers to employ GPUs' parallel processing to implement algorithms traditionally executed by CPUs.

## Coding

Options for submission, whichever is easier for you:

*   Copy your source into this repo, or...
*   Submit a PR for the Sprint Challenge from the `Computer-Architecture-One` repo
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
