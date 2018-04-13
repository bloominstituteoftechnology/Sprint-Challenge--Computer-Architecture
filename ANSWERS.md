<!-- Answers to the Short Answer Essay Questions go here -->

Short answer:

* Explain how the CPU provides concurrency:

Concurrent computing consists of process lifetimes overlapping, and execution doesn't have to be in the same instant. Concurrent processes can be executed via time-sharing slices. Multiple processes are part-way through execution at a single instant, but only one process is being executed at that instant. Concurrent computations may be executed in parallel on rarer occasions.

* Describe assembly language and machine language:

Assembly language is a low-level programming language for a programmable device that uses mnemonic sequences instead of numeric operation codes and can use symbolic lables instead of manually calculating offsets, fixed addresses or others.

Machine Language is only understood by computers and only consists of numbers. They are a set of instructions executed by a CPU. Each instruction performs a very specific task like an ALU operation. It is primitive, and can be compiled or assembled.

* Why is 3D performance so much higher with a graphics card than without?

GPUs were designed to deal with video, and developed so that the decoding process is handled by the GPU rather than the CPU. GPUs support 3-D rendering, animation, video editing, high-res displays, highly paralleled applications, BTC mining and VR.

* Suggest the role that graphics cards play in machine learning:

GPUs have hundreds of simpler cores, thousands of concurrent hardware threads, and maximizes floating-point throughput. Since a neural network is made up of multiple matrix multiplications, we can run all the operations at the same time instead of one after another. This trains the neural network faster. 
