<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency:
    A concurrent system is one where a computation can advance without waiting for all other computations to complete. It consist of process lifetimes overlapping, but execution need not happen at the same instant. In one core processes, time sharing slices can be done by interleaving the execution steps of each process.

2. Describe assembly language and machine language:
    Assembly language:
    Assembly language is a convenience mechanism over the machine language. With assembly language you use mnemonic sequences instead of numeric operation codes.It makes machine language more human readable.
     For example, for assembly language command PRN R0, the machine language would be: 01000011 00000000

    Machine language:
    Machine code or machine language is a set of instructions executed by a computer's CPU. Higher-level languages or assembly languages are translated into executable machine code by compilers, assemblers and linkers. Machine code are so unreadable.

3. Why is 3D performance so much higher with a graphics card than without?
    Graphics card provides a GPU, with thousands of computational cores, it works as a ancillary CPU, which reduces the load of CPU and let CPU to work on its usual tasks.


4. Suggest the role that graphics cards play in machine learning:
    GPU provides powerful and efficient parellel computing. With thousands of computational cores and 10-100x application throughput compared to CPUs alone, GPU has became the processor of choice for machine learning engineers. GPU are used to train deep neural networks using far larger training sets, in limited datacenter infrastructure, in less magnitude of time.