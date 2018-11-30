<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency or parallelism:  
- Concurrency and parallelism are two related but distinct concepts.
Concurrency means, essentially, that task A and task B both need to happen independently of each other, and A starts running, and then B starts before A is finished.
There are various different ways of accomplishing concurrency. One of them is parallelism--having multiple CPUs working on the different tasks at the same time. But that's not the only way. Another is by task switching, which works like this: Task A works up to a certain point, then the CPU working on it stops and switches over to task B, works on it for a while, and then switches back to task A. If the time slices are small enough, it may appear to the user that both things are being run in parallel, even though they're actually being processed in serial by a multitasking CPU.



2. Describe assembly language and machine language:
- Assembly language is a convenience mechanism over the machine language. With assembly language you use mnemonic sequences instead of numeric operation codes and can use symbolic labels instead of manually calculating offsets. It also protects you from really dumb errors - like typing a malformed processor instruction.
Otherwise, the assemly language is the equivalent of the machine language. Sometimes you will have an old assembler that will not support mnemonics for some instructions of the newer processors - then you can still insert operation codes directly into the program.

* Convert `11001111` binary

    to hex: CF

    to decimal: 207


* Convert `4C` hex

    to binary:  01001100

    to decimal:  76


* Convert `68` decimal

    to binary:  01000100

    to hex: 44

PR link for the 4 day project this week: see comments and also here: previous 4 days: LambdaSchool/Computer-Architecture#137
