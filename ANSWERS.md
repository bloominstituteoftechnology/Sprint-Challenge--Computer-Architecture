
## Binary, Decimal, and Hex

Complete the following problems:

* Convert `11001111` binary

    to hex: `1100 = C`, `1111 = F` => `0b11001111 = 0xCF`

    to decimal: `0b11001111=2^7+2^6+0+0+2^3+2^2+2^1+2^0=128+64+0+0+8+4+2+1=207`

* Convert `4C` hex

    to binary: `0b1001100`

    to decimal: `76`


* Convert `68` decimal

    to binary: `0b1000100`

    to hex: `0x44`


## Architecture


1. Explain how the CPU provides concurrency:

- Sinlge-core CPUs provide concurrency with timer interrupts that fire one time per second. When the interrupt occurs, CPU executes the interrupt handler. When it completes, the CPU returns to the previous state.

- Right answer: time slicing, multiple cores, multithreading.

2. Describe assembly language and machine language:

Machine language is represented by a sequence of hexadecimal numbers, bytes, the instructions given to CPU as to what to do next.

Assembly language is a more human readable version of the machine language, where the instructions and registers are represented by abbreviated names such as LDI, PRN, etc.

In `10011001    # LDI R0,8`, `10011001` is machine language and `LDI R0,8` is assembly language.

3. Why is 3D performance so much higher with a graphics card than without?

Graphics Cards (GPUs) are more efficient than CPUs. They process data in parallel, and have a lot more ALUs than CPUs.

4. Suggest the role that graphics cards play in machine learning:

Object recognition for self-driving cars, face recognition, big data analysis, etc. 
