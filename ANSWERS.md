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



```
------------------------

Convert `11001111` binary

to hex:
```md
`1100` `1111`

 12     15

 C      F

 hex = 0xCF
```
to decimal:
```
`11001111`

-------------------------
1 + 2 + 4 + 8 + 64 + 128

-------------------------
decimal = 207

```
* Convert 4C hex

    * to binary:

        ```
        _4_  == 0100

        _C_ == 1100

        binary = `01001100`
        ```

to decimal:
    
    4C = (4 × 16¹) + (12 × 16⁰)
       = 64 + 12
    decimal = 76
    

Convert 68 decimal

to binary:
```
68 = 0 + 64 + 0 + 0 + 0 + 4 + 0 + 0
binary = `01000100`
```
to hex:
```

68/16 = 4 R 4
First hex digit = 4

4/16 = 0 R 4

Second hex digit = 4

hex = 0x44

```

