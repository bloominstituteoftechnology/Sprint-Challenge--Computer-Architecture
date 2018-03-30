# Link to my code PR:
https://github.com/LambdaSchool/Computer-Architecture-One/pull/72



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
I'm not exactly sure about this - I did some reading on concurrent vs parallel computing and my understanding is that concurrent computing is when a processor is switching between multiple tasks to advance each a bit before switching to advance another, and so on. As to how I think this would be handled is through interrupts or a system like that where multiple programs may be running but the cpu is only actively handling any one program at a time. The states of the others are stored when they are not being executed.

In effect, programs A, B, and C are loaded. A is running first with B and C states saved. A bit of A is handled, then program A state is saved and program B state is loaded, a bit of B runs, then B state is saved and C state is loaded, a bit of C runs, then C state is saved and A state is loaded, etc.

* Describe assembly language and machine language:
Assembly language is human readable, and it is turned into machine language with the use of a compiler. Assembly language cannot be executed by the computer; it must be compiled into machine language, which the computer can execute. Assembly uses human-readable code comprised of mnemonics, operands, labels, and comments, while machine language is comprised of zeros and ones.


* Suggest the role that graphics cards play in machine learning:
The use of graphics cards has advanced machine learning greatly by increasing the number of operations that can be done at the same time. GPUs have a high number of cores, can handle thousands of concurrent hardware threads, and maximize floating point throughput. The analogy I read compared a CPU and GPU to a Ferrari and a truck. The Ferrari (CPU) is great for moving a small amount of things quickly, but the truck (GPU) moves much more even if it's at a slower pace. In the same amount of time, the truck (GPU) can haul a lot more than the Ferrari (CPU) can.


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

