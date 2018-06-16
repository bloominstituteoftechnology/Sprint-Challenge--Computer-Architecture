# Sprint-Challenge--Computer-Architecture

## Binary, Decimal, and Hex

Complete the following problems:

* **A.** Convert `11001111` binary
  ```
  128 + 64 + 0 + 0 + 8 + 4 + 2 + 1 = 207

  HEX: CF

  DECIMAL: 207
  ```

* **B.** Convert `4C` hex
  ```
  4 = 4, C = 12

  4 + 12 = 16

  4 * 16^1 = 64
  C * 16^0 = 12

  64 + 12 = 76

  BINARY: 1001100

  DECIMAL: 76
  ```

* **C.** Convert `68` decimal
  ```
  BINARY: 1000100

  HEX: 44
    1. divide 68 by 16 (base16) until the quotient is 0
        68 / 16 = 4 (remainer 4)
        4 / 16 = 0  (remainer 4)

    2. get the hex values for each remainder 
        decimal 4 = hex 4
        decimal 4 = hex 4

    3. combine the hex values in reverse order (last to first)
        hex 4 combined with hex 4 = hex 44
  ```


## Architecture

Short answer:

**1.** Explain how the CPU provides concurrency or parallelism:
  * Concurrency occurs when two tasks can execute, run, and complete in overlapping time periods. For example, loading multiple browser tabs at the same time. The cpu provides this added benefit by working independently of other cpu's within the same machine. 
  * Interrupts provide concurrency for CPU's as well. This allows the CPU to temporarily pause the execution of incoming instructions to handle a new event. 

**2.** Describe assembly language and machine language:
  * Machine language is essentially a series of binary numbers that represent instructions and data for the computer to read and execute.
  * Assembly language provides a more intuitive way to read and write machine language. The instructions and registers are given labels for simplified future reference. Assembly language translates directly to machine language.

**3.** Why is 3D performance so much higher with a graphics card than without?
  * "A CPU is used to handle all tasks that a computer needs to run - including running the operating system and assisting all the hardware in talking with each other - while a GPU is focused solely on graphics performance." -Kaitlyn
  * 3D cards utilize a Graphics Processing Unit, which is designed specifically for graphics-rendering computations. 

**4.** Suggest the role that graphics cards play in machine learning:
  * "Graphics cards allow rapid processing of visual data, which greatly accelerates machine learning when using data sets composed of pictures of real-world objects and situations." -Jared


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

