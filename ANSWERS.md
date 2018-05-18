
* Convert `11001111` binary

    to hex: CF

    to decimal: 207


* Convert `4C` hex

    to binary: 01001100

    to decimal: 76


* Convert `68` decimal

    to binary: 01000100


<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency:
> * the cpu stores instructions in it's registers. Particularly there are special registers called the Program Counter and the Instruction Register. The Program counter holds the address of the current instruction while the Instruction Register reads the instruction so that it can be processed.  There CPU also utilizes ram to store instructions or data that it doesn't need to process during a particular clock cycle. It can pop those instructions or data back off of the stack on RAM to pick up where it left off.


2. Describe assembly language and machine language:
> * __machine language__: opcodes written in 8bit binary which the CPU takes in directly as instructions and data.
> * __assembly language__: a layer of abstraction above machine language, it uses variable names to refer to opcodes. Essentially the assembly language and the machine language do the same thing, but assembly language is much more human readable.


3. Why is 3D performance so much higher with a graphics card than without?
> * graphics cards are able to process the complicated calculations needed for displaying graphics and can send the results directly to the peripherals like a monitor or a printer independently from the CPU.


4. Suggest the role that graphics cards play in machine learning:
> I'm honestly not certain of the answer to this question, but I imagine it has something to do with the fact that while a CPU usually only has one ALU graphics cards often have multiple ALUs (Arithmetic Logic Units) which allow them to make complicated calculations much quicker.
