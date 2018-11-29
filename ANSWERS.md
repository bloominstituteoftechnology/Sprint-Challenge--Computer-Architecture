<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency or parallelism:
    Concurrency or parallelism occurs only when several commands are executed at the same time without waiting for one or other command to finish before other starts. However computers only gives an illusion that they are excecuting programs at the same time. Remeber, CPUs are operating at vast speeds. In real life computers implement scheduling that gives right amount of speed and priority.This allows concurent processing or at least it looks to humans :D

2. Describe assembly language and machine language:
    Machine language is a collection of 0s and 1s, that is called a binary code and is meant to be read by the computers. On the other hand, assembly language, are the instructions that are easily readable by humans.


* Convert `11001111` binary

    to hex: (1100)(1111) => (8+4+0+0)(8+4+2+1) => (12)(15) => C F

    to decimal: 128+64+8+4+2+1 => 207


* Convert `4C` hex

    to binary: (4)(C) => (0100)(1100) => 01001100

    to decimal: 64+8+4 = 4x16 + 12x1 => 76


* Convert `68` decimal
    Action  Remainder
    2/68        0
    2/34        0
    2/17        1
    2/8         0
    2/4         0
    2/2         0
    2/1         1

    to binary: 01000100

    to hex: 68/16 = 4 (first digit)
            16/4 =  4(second digit)
            4/16 = 0 (stop here. result is 0)