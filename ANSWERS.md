<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency or parallelism:
The CPU can perform seemiimly multiple tasks at the same time through various technologies such as,  having multiple cores,  pipelining or hyperthreading.

Additionally, by using interrupts(time slicing), a single CPU can execute one program for a while (like 50 ms), and then switch to running another program for a while, parallelism can be simulated.

2. Describe assembly language and machine language:
Machine language (also known as machine code) is the series of bit patterns(binary o's and 1's) that provides instructions to a computer processor.
 instructions called  “op-code's” are numbers which might be followed in memory by “operands”, that is parameters for the op-code

 Assembly language is essentially the same as machine code,  instead of having to use/remember the op-codes, “mnemonics” are used(which are normally processor dependent..e.g Intel would differ from AMD for say each manufacturers 32 bit computers)which are slightly more human readable codes that map to the machine code. 
e.g.
LDA #$1F ; Load the hex value 1F into the accumulator

* Convert `11001111` binary

    to hex:
    1100 1111
    C     F

    to decimal:
    1  128
    1  64
    0
    0
    1  8
    1  4
    1  2
    1  1
      ----
      207


* Convert `4C` hex
4    c
0100 1100

    to binary:
0  0
1  64
0  0
0  0
1  8
1  4
0  0
0  0
   --
    76

    to decimal:
76

* Convert `68` decimal

0  0
1  64
0  0
0  0
0  0
1  4
0  0
0  0
-------

01000100

    to binary:
0100 0100
4     4
    to hex:

    44