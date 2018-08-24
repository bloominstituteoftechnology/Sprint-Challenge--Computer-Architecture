# Sprint-Challenge--Computer-Architecture

## Binary, Decimal, and Hex

Complete the following problems:

* Convert `11001111` binary

    to hex: 0xCF

    **WHY:**

        1100 1111
        12   15
        C    F

    to decimal: 207

    **WHY:**

        +---------- 128 place: 128 * 1 => 128
        |+--------- 64 place: 64 * 1    +  64
        ||+-------- 32 place: 32 * 0    +   0
        |||+------- 16 place: 16 * 0    +   0
        ||||+------ 8 place: 8 * 1      +   8
        |||||+----- 4 place: 4 * 1      +   4
        ||||||+---- 2 place: 2 * 1      +   2
        |||||||+--- 1 place: 1 * 1      +   1
        ||||||||                       ------
        11001111                          207 

* Convert `4C` hex

    to binary: 0b1001100

    **WHY:**

        4    C
        4    16
        0100 1100
        
    to decimal: 76

    **WHY:**

        +---- 16 place: 16 * 4 => 64
        |+--- 1 place: 1 * 12   + 12
        ||                     -----
        4C                        76
        
* Convert `68` decimal

    to binary: 0b1000100

    **WHY:**

        +---------- 128 place: 0 128's in 68 => 68
        |+--------- 64 place: 1 64 in 68     => 4
        ||+-------- 32 place: 0 32's in 4    => 4
        |||+------- 16 place: 0 16's in 4    => 4
        ||||+------ 8 place: 0 8's in 4      => 4
        |||||+----- 4 place: 1 4 in 4        => 0
        ||||||+---- 2 place: 0 2's in 2      => 0
        |||||||+--- 1 place: 0 1's in 0      => 0
        ||||||||
        01000100

    to hex: 0x44

    **WHY:**

        +---- 16 place: 4 16's in 68 => 4
        |+--- 1 place: 4 1's in 4    => 0
        ||
        44

## Architecture

Short answer:

* Explain how the CPU provides concurrency or parallelism:

  [See ANSWERS.md](ANSWERS.md)

* Describe assembly language and machine language:

  [See ANSWERS.md](ANSWERS.md)

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

