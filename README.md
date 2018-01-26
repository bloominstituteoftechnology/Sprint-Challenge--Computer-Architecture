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

* Explain how the CPU provides concurrency: A CPU provides concurrency through multithreading. Multithreading is a process where one thread runs until it hits an event which would cause a pause in the cycle time, at which point another thread utilizes the system resources. A basic example would be a process that has to access memory, which might take several CPU cycles. At that point another thread would run using those resources. 

* Describe assembly language and machine language: Machine language is the language used to issue direct commands to the CPU. Assembly language simply uses mnemonics in order to make the language easier for the programmer to read and write. 

* Suggest the role that graphics cards play in machine learning: A graphics card's use in machine learning is to offload the huge amount of operations involved from the CPU. The operations in machine learning can be massively parrallelized, and the GPU with it's thousands of cores is better able to process them.


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
00001010
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

