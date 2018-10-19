<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency or parallelism:

The CPU does provide concurrency by switching tasks super fast, it does context switching by getting interupted from a task that is higher priority then once the higher priority task is completed, it comes back to the task we were once on. It uses a scheduler in order to find what operations are top priority and in what order to execute them. Parallelism is different, it is when two tasks are executing at the exact same time, this can happen when multiple CPUs do completely different, non-dependent tasks simultaneously.
<!-- 
The difference between concurrency and parallelism
https://medium.com/from-the-scratch/dont-be-confused-between-concurrency-and-parallelism-eac8e703943a -->

1. Describe assembly language and machine language:

Machine language is what the computer can interpret. It converts those binary numbers to turn on/off the transistors in the order needed to perform an operation. Assembly language is a low level language, it has subroutines which are similar to functions but they don't allow arguments passed into the function. It uses mnemonic codes for instructions instead because it's easier for us humans to process and errors are reduced but the need for a complier is necessary to convert assembly code to machine code.