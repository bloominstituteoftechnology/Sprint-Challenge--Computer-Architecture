<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency:

Parallelism can be done by using mulitple cores (i.e. multiple cpus). Paralleism is impossible on a single core machine. However, concurrency - the overlapping of process lifetimes - can be accomplished on a single core computer using time sharing slices, which is essentially rapid cycles between disparate processes.

2. Describe assembly language and machine language:

Machine language is the actual language that the computer "understands" in order to execute a program, i.e. the bits of 1s and 0s that correspond to whether or not a transistor should contain voltage or not. Assembly language is a human readable programming language that is low-level i.e. close to machine language but not simply 1s and 0s, which the compiler compiles to machine language at runtime.


Code: https://github.com/LambdaSchool/Computer-Architecture/pull/19