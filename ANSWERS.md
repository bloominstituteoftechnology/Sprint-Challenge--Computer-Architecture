<!-- Answers to the Short Answer Essay Questions go here -->

David Loveday [9:24 AM]
### Binary, Decimal, and Hex
* Convert `11001111` binary
    to hex:
    to decimal:

`1100 1111`
   C   F
Base 16 addition: `8 + 4 + 2 + 1` is `F`, `8 + 4` is `C`.
 **Hex*: **`CF`**



`11001111`
128  64  0  0  8  4  2  1
For Demcimal:

 **Decimal**: **`207`**


 * Convert `4C` hex
     to binary:
     to decimal:

    to binary: `01001100`

    **to decimal**: **`76`**



    Convert `68` decimal

    **to binary*: **`01000100`**

    **to hex: `44`**


1. Explain how the CPU provides concurrency:

The CPU provides concurrency by way of task switching, which works like this: Task A works up to a certain point, then the CPU working on it stops and switches over to task B, works on it for a while, and then switches back to task A. If the time slices are small enough, it may appear to the user that both things are being run in parallel, even though they're actually being processed in serial by a multitasking CPU.


2. Describe assembly language and machine language:

Machine language is the lowest-level programming language. Machine languages are the only languages understood by computers. An assembly language contains the same instructions as a machine language, but the instructions and variables have names instead of being just numbers.


