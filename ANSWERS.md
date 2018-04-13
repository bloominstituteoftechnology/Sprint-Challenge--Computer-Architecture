<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency:
- In a simple CPU such as the LS-8, I believe it has to do with interrupts. When an interrupt is noted, the entire state of the CPU along with the tasks it's working on is saved, and the instruction inputted by the source of the interrupt is handled. After that, the CPU goes back to the task(s) it was working on before the interrupt took place, allowing for the concurrency.
 
2. Describe assembly language and machine language:
- Assembly Language is essentially written instructions for humans to read and reference, as well as being a low-level programming language that computer can understand. They refer to pieces of machine language. For example, if you had a segment of Machine Language that specifies a print, you would write that in Assembly Language as "PRT"

On the otherhand, Machine Language refers to instructions that are written explicitly in binary for the computer to understand. They are executed by the CPU and are used to carry out tasks.

3. Why is 3D performance so much higher with a graphics card than without?
- A graphics card is what's required to take the binary code from the CPU and convert it into pixels that can be put on the screen. Besides being a source of extra memory and processing power, it improves 3D performance so much because it's a highly specialized piece of equipment, whose (almost) sole purpose is to convert binary to an image. Without this, the CPU is forced to rely on what little processing power it has left over from carrying out all the other instructions to display images on the screen.

4. Suggest the role that graphics cards play in machine learning:
- I believe that GPUs are useful in Machine Learning because, unlike CPUs, they posses a few, extremely powerful and specific cores rather than tons and tons of cors that are multitaskers. Also, GPUs have many times more processors in them than a traditional CPU does.