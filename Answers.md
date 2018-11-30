## Binary, Decimal, and Hex

## Complete the following problems:

* Convert `11001111` binary

    to hex: CF  

    to decimal: 207

* Convert `4C` hex

    to binary: 01001100

    to decimal: 76


* Convert `68` decimal

    to binary: 1000100

    to hex: 44


## Architecture

One paragraph-ish:

* Explain how the CPU provides concurrency: Concurrency is a form of computing where several computations happen during overlapping time periods instead of sequentially. This can happen through shared memory or message passing. A CPU can perform concurrent computing even on a single core by time-sharing slices. Only one process is ran at a time, and if it isn't completed during its time, it pauses while another process begins, and then resumes at a later time. This keeps multiple processes partly through execution at a single instant, but does not allow more than one process to be executed at that instant. 

* Describe assembly language and machine language: Assembly Language is a human readable view of machine language. Instructions and registers are given mnemonics, and is very close to the machine language. Assembly language uses comments and labels instead of fixed memory addresses. Machine Language is the actual bits used to control the processor in the computer. The processor reads bits from program memory and those represent instructions for what to do next. Machine language gives a way to enter instructions in to a computer.

* Suggest the role that graphics cards play in machine learning: GPUs, or GPGPUs for the purpose of ML, offer a few advantages over CPUs in computational power, as well, they are much more affordable for that purpose. Most CPUs are designed to compute calculations in sequential order, so they are slower in instances such as neural networks, and the cost of CPUs with multiple cores are much higher than GPUs.


## Link To Code:
[Travis's LS-8 Build](https://github.com/LambdaSchool/Computer-Architecture-One/pull/66)
