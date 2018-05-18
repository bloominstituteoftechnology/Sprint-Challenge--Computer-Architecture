# Short Answer Essay Questions for Sprint Challenge Computer Architecture

## Number Conversion Exercises:

1.  Convert `11001111` binary

    * to hex: `CF`
    * to decimal: `207`

2.  Convert `4C` hex

    * to binary: `01001100`
    * to decimal: `76`

3.  Convert `68` decimal

    * to binary: `01000100`
    * to hex: `44`

## Short Answer:

1.  Explain how the CPU provides concurrency or parallelism:

    * For most single-core CPU's, code is initilized asynchronously, running new processes immediatly following the completion of the last. External components can be used, as in our ALU switch statement method, which takes on the responsiblity of any arithmetic calculations, freeing up the CPU to work on other things. Multi-core systems have the added ability to run code in parallel (at the same time), and not just sequentially.

2.  Describe assembly language and machine language:

    * Assembly language is the code that we humans write in, which is easier to read, but requires assembly/compilation in order to be read by a computer.
    * Assembly language code is compiled into machine code, which is easier to read by the computer, but harder to read for humans (unless you are Neo).

3.  Why is 3D performance so much higher with a graphics card than without?

    * Similar to our ALU switch statement method, graphics cards are designed with their own dedicated processors (GPU), to off-load processes like image rendering, and complete them really fast, so that the CPU doesn't have to.

4.  Suggest the role that graphics cards play in machine learning:
    * Graphics cards play a key role in machine learning, where the typical setup known as General Purpose Processing Unit (GPGPU) is necessary in order to be able to process complex algorithms involved replicating neural networks.
