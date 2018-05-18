<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency:
CPU concurrency is achieved with the use of a stack. This allows reference pointers or entire CPU states to be stored between ticks.

2. Describe assembly language and machine language:
Assembly language is a proramming language with a very close relationship to exactly what is happening in the computer. The sctest.asm file in this repo is assembly language for the test. Assembly language is compiled into machine language, which is the lowest level of programming we can see as programmers: it represents the exact calculations being made by the CPU, and is usually represented as binary or hex.


3. Why is 3D performance so much higher with a graphics card than without?
A GPU is a specialized type of CPU that is engineered to perform graphical operations efficiently. The engineering decisions required to optimise these operations diverge from those required to optimise traditional general purpose CPUs, so without a graphics card, graphics operations are by necessity of design handled inefficiently.

4. Suggest the role that graphics cards play in machine learning:
I'll swing back around to research this if I have more time, but my assumption would be that the operations being used at the machine language level of machine-learning programs have optimisation requirements that are more similar to those for graphics rendering than those for which traditional CPUs are optimised.