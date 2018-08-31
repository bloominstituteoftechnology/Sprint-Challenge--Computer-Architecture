<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency:
concurrent system is one in which it can execute several commands or processes at once without it waiting for another process or command to finish.   the CPU just has the ability to implement a MLFQ that provides just the right amount of turnaround time, priority and speed. This allows for concurrent processing instead of sequential processing.

2. Describe assembly language and machine language:
machine language are sequences of binary code that is meant to be read by the computer. Assembly code is code that is more human readable.
example:


Machine Code:

10101010 00000000 00000001
01000110 00000001


Assembly Language:

OR R0, R1
POP R1

Complete the following problems:

* Convert `1100 1111` binary // divide into two parts
         1100//12//C  1111// when you see all 1 //F
         #FF00FF
         #11111111 00000000 11111111

    to hex: CF
//11001111
 //128/    //8421 
    to decimal:207
    128 + 64 + 15


* Convert `4C` hex
//half it agaim
//0100 1100
    to binary: 01001100
//01= 64
//1 =8
//1= 4

    to decimal: 76


* Convert `68` decimal
//power of 2
68
64 + 4
1=0 128 PLACE
2= 1 IN THE 64THPLACE
3= 0 IN THE 32TH PLACE
4= 0 IN 16TH PLACE 
5=0 IN 8TH PLACE
6 = 1 IN 4TH
7= 0 IN 2'S PLACE
8 = 0 IN ONE PLACE
// IF A 1 IS IN THE LAST IT MUST BE ODD
01000100 
    to binary:01000100

    to hex: 44
