<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency:

The CPU provides concurrency with the Direct Memory Access (DMA). This allows the CPU to work on computations while the DMA accesses RAM for any peripherals. Another way the CPU could have concurrency is by having multiple processing cores.

2. Describe assembly language and machine language:
Machine language are the actual bits used to control the computer. It is written in binary (Base 2)Each 1 represents on and each 0 represents off. They are usually represented in hexadecimal to humans and grouped into bytes. 
 
Assembly language is a more human readable form of machine language. It gives names to different instructions (such as "dec r1"). This helps reduce human error. It must be run through an assembler to be read by the computer.



3. Why is 3D performance so much higher with a graphics card than without?

Graphics cards offload some work from the CPU, specifically quickly computating complex problems. They specialize at processing large blocks of data in parallel. 



4. Suggest the role that graphics cards play in machine learning:

Because graphics cards work well with parallel data they can feed in many inputs to a nerual network at once, greatly decreasing the amount of time it takes to teach the machine.

