<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency:
- a concurrent system is one in which it can execute several commands or processes at once without it waiting for another process or command to finish. This gives the illusion that all the magic is happening at once...when in reality the CPU just has the ability to implement a MLFQ that provides just the right amount of turnaround time, priority and speed. This allows for concurrent processing instead of sequential processing...which would eat time and potentially money. 


2. Describe assembly language and machine language:

- machine language are sequences of binary code that is meant to be read by the computer. Assembly code is code that is more human readable. 

__example:__

```

Machine Code:

10101010 00000000 00000001
01000110 00000001


Assembly Language:

OR R0, R1
POP R1



