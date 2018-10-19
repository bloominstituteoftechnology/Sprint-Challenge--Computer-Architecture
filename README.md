# Sprint-Challenge--Computer-Architecture

## Binary, Decimal, and Hex

Complete the following problems:

* Convert `11001111` binary

    to hex: CF

    to decimal:207


* Convert `4C` hex

    to binary: 01001100

    to decimal: 76


* Convert `68` decimal

    to binary: 01000100

    to hex: 44


## Architecture

Short answer:

* Explain how the CPU provides concurrency or parallelism:
It seems that the CPU does stuff off of a clock. This clock is something that rises and falls a few billion times per second. That means the CPU can do a lot of things in 1 second. I believe what is happening is that CPU is the greatest multi-tasker on the planet. Let's say it's doing A, and get's told to do B. It pauses A and does B. When finished with B it goes back to A and finishes A. Now instead of just A and B, the CPU can do this for A - Z times a million! It achieves this by placing stuff on the stack and using pointers to keep track of where it left off on the previous process. The extremely fast switching gives the appearance of concurrency or parallelism while at the nano-scale is merely time-sharing. For real concurrency (where let's say 10 things happen on 1 clock cycle), we need to move from C to VHDL and use FPGAs instead of CPUs!

* Describe assembly language and machine language:
I believe machine language is strings of zeros and ones. It corresponds to voltages on a wire (electricity) and is the natural language of hardware. While writing strings of zeros and ones is probably where software started, it's not exactly the easiest thing to read for humans. (I'm sure electrons would feel that machine code is a very high level language). In order for us humans to come up with some a little more readible, a partial machine code compiler was created to compile Assembly code into machine code. Assembly code is a significant step up from machine code in terms of human readability. It provides a set of instructions that tell the CPU what to do. These instructions are usually some command followed by 0 or more some operands. The instruction set usually drives what these instructions are for the CPU and can be found in a spec. These instructions formed the basis of programming languages and were modeled aftera a Turing machine. The turing machine was an idea that you could take a tape and place data on it and jump around to any other portion of the tape. In the advent of the CPU, this was a memory stack. In 2018, Assembly is considered a low-language, however I wonder how many would agree, that a language's level of abstraction is purely based on time and perspective (human vs an electron)! :)


## Coding

Options for submission, whichever is easier for you:

* Copy your source into this repo, or...
* Submit a PR for the Sprint Challenge from the `Computer-Architecture` repo
  you've been using all along.

Sprint Challenge:

[See the LS-8 spec for details](https://github.com/LambdaSchool/Computer-Architecture/blob/master/LS8-spec.md)

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

10000010 # LDI R0,10
00000000
00001010
10000010 # LDI R1,20
00000001
00010100
10000010 # LDI R2,TEST1
00000010
00010011
10100111 # CMP R0,R1
00000000
00000001
01010101 # JEQ R2
00000010
10000010 # LDI R3,1
00000011
00000001
01000111 # PRN R3
00000011
# TEST1 (address 19):
10000010 # LDI R2,TEST2
00000010
00100000
10100111 # CMP R0,R1
00000000
00000001
01010110 # JNE R2
00000010
10000010 # LDI R3,2
00000011
00000010
01000111 # PRN R3
00000011
# TEST2 (address 32):
10000010 # LDI R1,10
00000001
00001010
10000010 # LDI R2,TEST3
00000010
00110000
10100111 # CMP R0,R1
00000000
00000001
01010101 # JEQ R2
00000010
10000010 # LDI R3,3
00000011
00000011
01000111 # PRN R3
00000011
# TEST3 (address 48):
10000010 # LDI R2,TEST4
00000010
00111101
10100111 # CMP R0,R1
00000000
00000001
01010110 # JNE R2
00000010
10000010 # LDI R3,4
00000011
00000100
01000111 # PRN R3
00000011
# TEST4 (address 61):
10000010 # LDI R3,5
00000011
00000101
01000111 # PRN R3
00000011
10000010 # LDI R2,TEST5
00000010
01001001
01010100 # JMP R2
00000010
01000111 # PRN R3
00000011
# TEST5 (address 73):
00000001 # HLT
```

