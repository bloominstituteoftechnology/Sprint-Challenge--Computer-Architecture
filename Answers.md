## Binary, Decimal, and Hex

Complete the following problems:

* Convert `11001111` binary

    to hex: 0b11001111.toString(16); -> 0xCF

    to decimal: parseInt(0b11001111); -> 207


* Convert `4C` hex

    to binary: 0x4c.toString(2); -> 0b01001100

    to decimal: parseInt(0x4c); -> 76


* Convert `68` decimal

    to binary: 68..toString(2); -> 0b01000100

    to hex: 68..toString(16); -> 0x44


## Architecture

One paragraph-ish:

* Explain how the CPU provides concurrency:
  * By using the interrupt the CPU is able to briefly halt one program as it runs another program. Once that program is done it can return to the previous program without affecting the final outcome of the program/algorithm. 

* Describe assembly language and machine language:
  * Assembly language is the language most instructions by humans are written in. Assembly languages, such as JavaScript, are then translated into machine language, binary, by a compiler. The CPU uses machine language to run programs and algorithms. 

* Suggest the role that graphics cards play in machine learning:
  * GPUs have a massively parallel architecture consisting of thousands of smaller, more efficient cores designed for handling multiple tasks simultaneously. The design of graphic cards makes them suitable for machine learning. 