# Binary, Decimal, and Hex
## Question: Convert 11001111 binary
* Hex: CF
* Decimal: 207

## Question: Convert 4C hex
* Binary: 0b01001100
* Decimal: 76

## Question: Convert 68 decimal
* Binary: 0b01000100
* Hex: 0x44

# Architecture
## Question: Explain how the CPU provides concurrency.
* By utilizing individual process stacks, the CPU can permit other CPU threads to execute simultaneously without affecting each other. Each thread executes on its own stack. However not all instructions can be executed concurrently. For example loading things into memory can be done concurrently, however performing mathematical calculations based on the values those memory locations hold can't, because each calculation essentially depends on the previous one being finished. The CPU also uses the stack for allowing the user to simultaneously interrupt the processes and input new instructions, this is handled through the Interrupt Requests sent from IO modules like the keyboard and mouse, etc. After the user is finished with the instruction, the CPU picks up where it left off by re-loading from stack the CPU state it was on when it was interrupted.
## Question: Describe assembly language and machine language.
* Assembly language is a more human readable language that represents op codes, and register flags. Assembly language uses a mnemonic method to represent the instructions and op codes, and the code written in this language is then interpreted by an assembler program that converts it into executable machine language code.
* Machine Language is code and instructions designed to be executed directly by the CPU. It is the lowest level of programming language. Machine language is represented in 1s and 0s, and it is binary in nature, but each processor type and manufacturer uses its own set of instructions.
## Question: Suggest the role that graphics cards play in machine learning.
* A GPU can handle multiple parallel instructions at once much more than a CPU can, and this is what the GPU is optimized for, because of this and the nature of machine learning which requires multiple matrix multiplications and other operations can be computed in parallel, and while a CPU with 12 or 18 cores can process 1 of those operations very quickly, the GPU with thousands of slower cores at its disposal, and fast memory access still beats the CPU.