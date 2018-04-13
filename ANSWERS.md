<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency: One way that I know of is by pushing the program counter and any necessary variables (like flags) onto the stack in ram so that a subroutine or interrupt can be handled before then popping that data back off the the stack and resetting the program counter back to where it should be.


2. Describe assembly language and machine language: Machine language is just ones and zeros, while assembly language is english shorthand representing what the machine language is doing.


3. Why is 3D performance so much higher with a graphics card than without? The graphics card provides a GPU, a graphics processing unit, that works as an ancillary aid to the CPU. In so doing, it allows the intense load of the graphics processing to be done apart from the CPU which is then freed to work on its usual tasks.


4. Suggest the role that graphics cards play in machine learning: GPUs are much faster than CPUs, and because ML requires tremendous processing power, GPUs allow for this to be done more easily.


## Sprint Repo:
https://github.com/LambdaSchool/Computer-Architecture-One/pull/100