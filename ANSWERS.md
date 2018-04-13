<!-- Answers to the Short Answer Essay Questions go here -->

Binary, Decimal, and Hex

Complete the following problems:

    Convert 11001111 binary

    to hex: 0xCF | JavaScript: (0b11001111).toString(16)

    to decimal: 207 | Javascript: parseInt('11001111', 2)

    Convert 4C hex

    to binary: 0b01001100 | JavaScript: (0x4c).toString(2)

    to decimal: 76 | JavaScript: parseInt('4c', 16)

    Convert 68 decimal

    to binary: 0b01000100 | JavaScript: (68).toString(2)

    to hex: 0x44 | JavaScript: (68).toString(16)

Architecture

1. Explain how the CPU provides concurrency:

  CPU provide concurrency though having dfferent processes that are executed by the CPU at the same time in multiple threads of control. 
	Different processes are executed by CPU alternately. However, when the CPU is executing one process, other processes are suspended and all states
	are saved to the stack. When interrupts are called in the CPU, the executing process is suspended and the triggered process will load all the states
	from the stack and resume execution. If the CPI has multiple cores to execute different processes, then the efficiency of concurrency will be much higher 
	than a CPU with only one core.

2. Describe assembly language and machine language:
   Assembly language is a low-level programming language for a computer, or other programmable device, in which there is a very strong correspondence between the 
	 language and the architecture's machine code instructions. There are multiple assembly languages that is specific to a particular computer architecture. Assembly is different from
	 high-level programming languages which are generally portable across multiple architectures but require compiling. Assembly language is converted into executable machine code though an assembler. 
   Assembly language uses mnemonic sequences to represent each low-level machine instruction or opcode, instead of numeric operation code and use symbolic labels instead of manually calculating offset. 
	 Machine language is a set of instructions executed directly by a CPU. Assembly is the lowest-level representation of a compiled or assembled computer program.  
	 
3. Why is 3D performance so much higher with a graphics card than without?
   Graphics cards provide a GPU (Graphical processing Unit) that works as an aid to the CPU, and eases the load of graphics processing so that the CPU does not have to do its usual tasks and graphics processing at the same time.
	 GPUs are also very necessary due to the fact that they have multiple cores that are able to process data simultaneously and most video games now have advanced 3D graphics.

4. Suggest the role that graphics cards play in machine learning:
   
   Graphics cards have thousands of simpler cores thousand of concurrent hardware threads that can maximize floating-point throughput and do simple mathematical operations in parallel. They are also used for
	 neutral network deep learning since GPU performs tasks in parallel whereas CPU executes one task at a time. GPUs are also boosting the deep learning field. GPUs also allow much larger data sets to be processed in a shorter amount of time with less infrastructure as well. 
