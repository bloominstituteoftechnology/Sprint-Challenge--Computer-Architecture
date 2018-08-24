# Sprint-Challenge--Computer-Architecture

## Binary, Decimal, and Hex

Complete the following problems:

- Convert `11001111` binary

  to hex:
  1100 1111 = CF
  8*1 + 4*1 + 2*0 + 1*0 + 8*1 + 4*1 + 2*1 + 1*1 = CF

  to decimal:
  11001111 = 207
  128*1 + 64*1 + 32*0 + 16*0 + 8*1 + 4*1 + 2*1 + 1*1 = 207

* Convert `4C` hex

  to binary:
  4 C = 01001100
  0100 1100 = 01001100

  to decimal:
  4 C
  16*4 + 1*12 = 76
  64 + 12 = 76

- Convert `68` decimal

  to binary:

  1. 68 / (2^6 = 64) = 1,06.. -> 68 is CONTAINED in a 7-digit binary number -> 0b1000000
  2. 68 % (2^6 = 64) = 4
  3. 4 / (2^2 = 4) = 0 -> 4 is CONTAINED in a 3-digit binary number -> 0b100
  4. 4 % (2^2 = 4) = 0
  5. 0b1000000 & 0b100 = 0b1000100 (RESULT)

to hex:

1. 68 / (16^1 = 16) = 4,25 -> 68 is CONTAINED in a 2-digit hex number -> 0x40
2. 68 % (16^1 = 16) = 4
3. 4 / (16^0 = 1) = 4 -> 4 is CONTAINED in a 1-digit hex number -> 0x4
4. 0x40 & 0x4 = 0x44 (RESULT)

## Architecture

Short answer:

- Explain how the CPU provides concurrency or parallelism:

* CPU in conjunction with the RAM is able to manage several processes (of course the Scheduler has an important role), CPU by itself is amnesic, it only knows what it is doing at the current instance of time (current cycle), no more far away. In order to provide some mnemonic help, the CPU get help from the RAM's big mnemonic capabilities. At the time the CPU has to handle a totally different Process/Thread, it passes 'all its current execution state' to the RAM, and trust blindly the RAM will take care of that information/state. The RAM puts that info pilled down into the Stack queue (a general queue or at the Stack that belongs to that specific Process/Thread that the CPU was running). This architecture enables a single-tasking CPU to run several numbers of different Processes/Threads.

- Describe assembly language and machine language:

* Machine Language: Is a non human-friendly representation of tasks to be performed sequentially, one after the other. Just 1's and 0's with an specific meaning.
* Assembly Language: Is a layer of abstraction on top of Machine Lnaguage (ML) that makes the ML more human-friendly, in a way the we as developers can 'write' instruction using the symbols we are confortable with (Alphabeth, in that case), representational symbols we can understand in an easy way.

## Coding

Options for submission, whichever is easier for you:

- Copy your source into this repo, or...
- Submit a PR for the Sprint Challenge from the `Computer-Architecture-One` repo
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
