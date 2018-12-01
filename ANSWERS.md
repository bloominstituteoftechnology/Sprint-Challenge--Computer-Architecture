<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency or parallelism:

   The CPU can do multiple things at once through using multiple cores or hyperthreading which is the hardware of a computer. In addition, CPUs use
   threads which work on a single cpu to execute one program at a time. This would be the OS.

2. Describe assembly language and machine language:
   -Machine language is primarily bit patterns which are 1s and 0s which only
   the cpu can understand. The cpu uses it to execute instructions. Assembly language is human readable code that uses mnemonics to make it easier
   to navigate through the code. The assembler turns assembly code into machine code.

---

- Convert `11001111` binary

1 _ 1 = 1
1 _ 2 = 2
1 _ 4 = 4
1 _ 8 = 8
0 _ 16 = 0
0 _ 32 = 0
1 _ 64 = 64
1 _ 128 = 128

---

        = 207

1100 1111
12 15
C F
to hex: 0xCF

## to decimal: 207

- Convert `4C` hex
  4 C
  0100 12
  1100

  to binary: 0b01001100
  0 _ 1 = 0
  0 _ 2 = 0
  1 _ 4 = 4
  1 _ 8 = 8
  0 _ 16 = 0
  0 _ 32 = 0
  1 _ 64 = 64
  0 _ 128 = 0

  to decimal: 76

---

- Convert `68` decimal

0 _ 1 = 0
0 _ 2 = 0
1 _ 4 = 4
0 _ 8 = 0
0 _ 16 = 0
0 _ 32 = 0
1 _ 64 = 64
0 _ 128 = 0

to binary: 0b01000100

0100 0100
4 4

to hex: 0x44
