<!-- Answers to the Short Answer Essay Questions go here -->

1.  Explain how the CPU provides concurrency:
    Concurrency is when two or more tasks run and are completed in overlapping periods of time. An example would be multi-tasking on a single-core machine. This differs from parallelism where tasks run at the same time on a multi-core processor.

2.  Describe assembly language and machine language:
    Machine Language is simply binary code. The computer can only read and execute tasks in binary form. It is also the lowest level of programming.

Assembly language is the next level up from machine code. It's the lowest level human-readable language, and it uses mnemonic names and symbols vs. raw sequences of bits.

3.  Why is 3D performance so much higher with a graphics card than without?
    A GPU is composed of hundreds of cores that can handle many thousands of threads simulteously whereas CPUs are designed with just a few cores and can only handle a few threads at a time. Both have their uses cases, but GPUs are faster and more efficient at complex math processes, which is at the center of rendering 3D graphics. In this way, a GPU is going to give much better performance for 3D.

4.  Suggest the role that graphics cards play in machine learning:
    Similarly, machine learning is computationally intensive because a neural network is comprised of multiple matrix multiplications. To train a neural network faster it is more efficient to run all of the operations at the same time vs doing one after the other. This is why a GPU would be better at a CPU for machine learning.
