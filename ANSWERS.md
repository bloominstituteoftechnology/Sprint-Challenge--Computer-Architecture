<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency:
A single-core CPU like the ls8 emulator we made this week provides concurrency through task switching, by pushing the PC location as a value to the stack, completing a subroutine, then returning to the position from which that subroutine was called in order to continue executing instructions linearly. A multi-core cpu would also employ parallelism to accomplish concurrent processing simulataneously.

2. Describe assembly language and machine language:
Assembly language is a somewhat human-readable language that is very close to machine language, with only a thin level of abstraction in the form of names for instructions and registers. Machine language refers to the binary or hex information that a computer can respond to directly as changes of physical state or voltage. Since a circuit can only be open or closed, on or off, all interactions with a computer must be reduced to this type of 

3. Why is 3D performance so much higher with a graphics card than without?
Graphics cards have hundreds of cores that simultaneously process sets of instructions that are very small compared to that which a CPU processes. This makes it uniquely capable of handling video rendering, which relies on the delivery of large groupings of problems to be solved in parallel.


4. Suggest the role that graphics cards play in machine learning:
While conventional computing relies on mechanical processes modeling deductive logic, machine learning depends more on iterative processes of interaction and induction. This makes parallel processing of smaller logical operations ideal for machine learning.