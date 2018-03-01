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
  
 Intel Hyper-Threading Technology runs 2 pipelines of instuction processing simultaneously to simulate 2 virtual cores.   
 Hardware and software interupts simulate concurrency.
 Modern PCs have multiple physical cores.


* Describe assembly language and machine language:

  Assembly language has mnemonics for machine language numeric codes.  There is a one-to-one correspondance except for macros from Macro Assemblers.  

* Suggest the role that graphics cards play in machine learning:

  Graphics cards excel at parallel array processing and in general have more processing power than the CPU.  Driven by AI and machine learning demands NVIDIA GPU processing speeds are increasing faster than Moore's Law and outpacing CPU development.   


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
