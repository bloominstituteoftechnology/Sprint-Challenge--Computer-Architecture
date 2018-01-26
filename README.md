# Sprint-Challenge--Computer-Architecture

## Binary, Decimal, and Hex

Complete the following problems:

* Convert `11001111` binary

    to hex: CF

    to decimal: 207


* Convert `4C` hex

    to binary: 1001100

    to decimal: 76


* Convert `68` decimal

    to binary: 1000100

    to hex: 44


## Architecture

One paragraph-ish:

* Explain how the CPU provides concurrency: The CPU provides concurrency by managing operations for more than one process on the single CPU. Many operations may be happening at once but as the CPU can only perform a single calculation at a time it can juggle executing slices of different operations concurrently thereby apearing to perform multiple tasks concurrently. 

* Describe assembly language and machine language: Assembly language is a more easily readable version of machine language. As the machine can only read 1/0 these series of commands an assembly language can be used to assign names to operands instead of raw digits.

* Suggest the role that graphics cards play in machine learning: Graphics cards play a critcal role in machine learning because by thier nature of having large numbers of small cores and large in board cashes they can perform huge numbers of calculations in an equal time compared with even a multicore CPU. GPU is no threat to the CPU for general computing but in regards to AI they offer huge advantages by allowing massive quantities of data to be processed simultaneously. 


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

