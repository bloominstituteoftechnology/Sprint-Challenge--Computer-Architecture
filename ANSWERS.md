<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency:
    The CPU provides concurrency through a process called timesharing. What this boils down to is the OS kernel has a part called the scheduler which does exactly what it sounds like, it manages when each program will get it's face time with the CPU. The CPU runs in extremely quick waves called clock cycles, during each of which many machine instructions can take place. The scheduler has the ability to pause processes in motion, switch to another process, and then switch back to the pauses process. This procedure happens so quickly that to us mere mortals it can appear as though both processes are happening concurrently, when this in fact is not the case. 


2. Describe assembly language and machine language:
    Assembly language and machine language both represent the specific instructions that the CPU needs in order to execute a process. The difference is that assembly language is still abstracted a bit (though much less than high level coding languages) to include human readable names and numbers to represent the instruction. While these names can be very esoteric, it is still doable to code in assembly language, however assembly language is (usually) the product of a compiled higher level coding language. Machine language, however, consists solely of bytes that the machine can read and execute. All assembly code is eventually compiled into machine language, which would be very difficult to code in directly. 

