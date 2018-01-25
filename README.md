# Sprint-Challenge--Computer-Architecture

## Binary, Decimal, and Hex

Complete the following problems:

* Convert `11001111` binary

    to hex:

    to decimal:


* Convert `4C` hex

    to binary:

    to decimal:


* Convert `68` decimal

    to binary:

    to hex:


## Architecture

One paragraph-ish:

* Explain how the CPU provides concurrency:

* Describe assembly language and machine language:

* Suggest the role that graphics cards play in machine learning:


## Coding

[See the LS-8 spec for details](https://github.com/LambdaSchool/Computer-Architecture-One/blob/master/LS8-SPEC.md)

Add the `CMP` instruction and `equal` flag to your LS-8.

Add the `JMP` instruction.

Add the `JEQ` and `JNE` instructions.

(You can just issue a PR from whichever repo you cloned for LS-8. Or you
can copy files in here and issue a PR for this one.)


[Here is some code](sctest.ls8) that exercises the above instructions. It should
print 1, then 4, then 5.

```javascript
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

