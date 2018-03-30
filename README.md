# Sprint-Challenge--Computer-Architecture

## Binary, Decimal, and Hex

Complete the following problems:

* Convert `11001111` binary

    to hex: 0xCF

    to decimal: 207


* Convert `4C` hex

    to binary:  0b01001100

    to decimal: 76


* Convert `68` decimal

    to binary:  0b01000100

    to hex: 0x44


## Architecture

One paragraph-ish:

* Explain how the CPU provides concurrency:

    The CPU provides concurrency in a few different ways. In a single-core CPU, where parallel computing is not possible, concurrency is achieved by the CPU keeping track of multiple processes running and sequentially initiating them so their lifetimes overlap, but the execution does not. Multiple components such as an ALU to offset duties of the CPU also add to concurency so multiple, sequential processes can be executed by the CPU. In a multi-core CPU, it can be provide from parallel and concurrent computing by literally executing multiple processes at once. Another way the CPU provides concurrency is through asynchronous actions, going into more detail from the explanation above, an event may not finish before the next one begins and they can all complete at different times (or must complete at different times in a single-core situation). 

* Describe assembly language and machine language:

    Assembly language is the language with which humans write machine code using legible sequences that we can more easily keep track of, like giving variable names to machine opcodes and using comments to keep track of its description. Its main goal is human legibility while still able to execute on a machine, just simplified to make it easier on the eye. Machine language is purely opcodes, bit encoding of the opcodes, typically in binary or hexadecimal format. It would take a spec and time spent translating to figure out how to read machine language by itself. 

* Suggest the role that graphics cards play in machine learning:

    GPUs play a significant role in machine learning. They are much more efficient than CPUs when it comes to concurrent processing due to their having many simpler cores (multiple thousands), whereas a CPU would have a small (<16 typically, highest server CPUs having ~28) amount of complex cores. CPUs execute sequentially waiting on previous processes in relation to machine learning and complete operations in much longer timeframes. GPUs can execute the same processes in parallel at a much faster pace. There are even specific GPUs called GPGPUs (General Purpose Graphics Processing Unit) for this purpose. 


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

