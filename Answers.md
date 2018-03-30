## Binary, Decimal, and Hex

Complete the following problems:

* Convert `11001111` binary

    to hex: cf

    to decimal: 207


* Convert `4C` hex

    to binary: 1001100

    to decimal: 76


* Convert `68` decimal

    to binary: 1000100

    to hex: 44


## Architecture

One paragraph-ish:

* Explain how the CPU provides concurrency:
  * The CPU has evolved to enable concurrent processing of program instructions and data. It now has multiple cores to compute with, supplementing the Arithmetic Logic Unit. This, combined with caches for memory, Direct Memory Access, and the RAM itself allow modern computers to vastly improve the throughput of the overall system through the study of concurrent computing. 

* Describe assembly language and machine language:
  * Assembly language - In this week's work, our `.js` files (`cpu.js`, `ram.js`, etc.) served as assembly language. They dealt with the mnemonics of low-level programming and told kept track of Registers, Stack Pointers, and Program Controllers. Assembly language and Machine language programs are both typically specific to a certain hardware. In this case, our simulated 8-bit CPU.
  * Machine language - The `.ls8` files served as machine language. They provided the raw binary code instructions carried out by the simulated hardware. 

* Suggest the role that graphics cards play in machine learning:
  * Graphic cards have evolved to include very specialized CPUs in addition to their own RAM. The calculations needed to provide high end graphics for games and design work are similar to those in a machine learning environment. GPU computing (Source: nvidia) is now enabling individuals and institutions to train models using massive data sets without renting space in a data center. 

