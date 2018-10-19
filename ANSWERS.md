# Sprint-Challenge--Computer-Architecture

## Binary, Decimal, and Hex

Complete the following problems:

* Convert `11001111` binary

Binary:     11001111

Solution:   128 64 32 16 8 4 2 1
            11001111
            128+64+8+4+2+1

Decimal:    207

Solution    8421 8421
            1100 1111
            12 15        

Hex:        C F

    to hex:         CF

    to decimal:     207


* Convert `4C` hex

Solution:

Hex:        4C
            4 12
            0100 1100

    to binary:  01001100

    to decimal: 76


* Convert `68` decimal

    to binary:  0100 0100

    to hex:     44


<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency or parallelism:

In reality, the CPU cannot run multiple programs at the same time because this will slow down the performance of the CPU. However, it does create the illusion that several programs are all running at the same time because the CPU's scheduler which is able to run program after program seamlessly and switching between programs in terms of nano seconds which is very fast to be observed.     

2. Describe assembly language and machine language:

Assembly Language which is human readable language. Examples are PRN for print, HALT for halt, LDI for load immediately, etc. Machine language is language understood by machines. This is usually binary in nature which is express in '1' and '0'.


Note: The implementation of the codes are found in this Pull Request for the Computer Architecture Repo: 

https://github.com/LambdaSchool/Computer-Architecture/pull/37 