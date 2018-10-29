<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency or parallelism:

Consider that a CPU wants to run program A and program B concurrently. The CPU provides concurrency by partially evaluating the instructions associated with program A a small portion at a time, switching to partially evaluating the instructions of program B a small portion at a time, and then switching back to continue to evaluate the instructions associated with program A. This continues until the instructions for programs A and B are both fully evaluated, giving the illusion that the instructions associated with each program were evaluated in parallel when they were really processed one instruction at a time. 

2. Describe assembly language and machine language:

Assembly language is a lower level language that used to program programs at the level right before the CPU level, and by that I mean programs that specify what CPU-level instructions/operation should be carried out at a given time and the operands on which to carry out the instructions/operations, if applicable. Assembly language gets compiled to machine language. 

Assembly language gets compiled to machine language. The instructions specified in the assembly code are converted to a machine language per the instructions of the CPU architecture (e.g. x-86 architecture, ARM A64 architecture). The instructions of the CPU architecture tell you the binary number associated with the different instruction opcodes and the binary numbers associated with the CPU registers. The binary numbers associated with a specific instruction (including the binary numbers specifying the location/address of the operands, if applicable) are fed as a CPU word to the CPU and any output values of the instruction are set/stored to a specified register.

