* Convert `11001111` binary
1)  to hex: 0xCF
1)  to decimal: 207

* Convert `4C` hex
1) to binary: 01001100
1) to decimal: 76

* Convert `68` decimal
1) to binary: 1000100
1) to hex: 0x44

* Explain how the CPU provides concurrency:
> The processor reads a sequence of instructions contained in the memory and executes them. 
> This sequence of commands is called a program and represents the algorithm of the processor. 
> The priority of reading commands changes if the processor reads the jump command, 
> then the address of the next instruction may be different. Another example of a process change is 
> the case of a stop command being received or switching to interrupt handling mode.

#### Stages of the execution cycle:
1) The processor sets the number stored in the instruction counter register to the address bus and gives the read command to the memory.
1) The number is the memory address; The memory, having received the address and the read command, exposes the contents stored at this address to the data bus and reports the readiness.
1) The processor receives a number from the data bus, interprets it as a command (machine instruction) from it's command system and executes it.
1) If the last command is not a jump command, the processor increments by one (assuming the length of each command is one) the number stored in the command counter; as a result, the address of the next command is formed there.
> The speed of the transition from one stage of the cycle to another is determined by the clock generator. 
> The clock generator generates pulses that serve as a rhythm for the CPU.
> Using pipelining, increasing the number of processor functional units (superscalar(arithmetic and logic unit, floating 
> point unit, integer multiplier, integer shifter)), increasing the number of cores (multicore), and increasing 
> the number of processors (multiprocessing) are different ways to increase productivity, which can be used together.

* Describe assembly language and machine language:
> Machine language is a collection of binary digits or bits that the computer reads and interprets.
> Machine language is the only language a computer is capable of understanding.
> Assembly language is a more human readable view of machine language.
> Instead of representing the machine language as numbers, the instructions and 
> registers are given names (typically abbreviated words, or mnemonics).
> Unlike a high level language, assembler is very close to the machine language.

* Suggest the role that graphics cards play in machine learning:
> Data scientists have been using GPUs for machine learning to make improvements across a variety of 
> applications including image classification, video analytics, speech recognition and natural language processing.
> A simple way to understand the difference between a GPU and a CPU is to compare how they process tasks.
> A CPU consists of a few cores optimized for sequential serial processing while a GPU has a massively parallel 
> architecture consisting of thousands of smaller, more efficient cores designed for handling multiple tasks 
> simultaneously.