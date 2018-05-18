<!-- Answers to the Short Answer Essay Questions go here -->
## Binary, Decimal, and Hex

Complete the following problems:

* Convert `11001111` binary

    to hex: `CF`

    to decimal: `207`


* Convert `4C` hex

    to binary: `01001100`

    to decimal: `76`


* Convert `68` decimal

    to binary: `01000100`

    to hex: `44`


1. Explain how the CPU provides concurrency:

*   The modern CPU provides concurrency, which in this instance is defined as taking advantage of the decomposability of software into subunits capable of being run in parallel. This concurrency is achieved through multiple cores (dual core, quad core, etc) and threading. It also can entail multiple CPUs on the same motherboard. 

2. Describe assembly language and machine language:

*   Assembly language is a low-level programming language that is similar to the machine code of the specific architecture it is utilized on. An assembler converts assembly language to machine language, which is performed directly on the CPU at the lowest level accessible to the programmer, colloquially known as "zeros and one" or "binary code" in common speak.

3. Why is 3D performance so much higher with a graphics card than without?

*   3D performance is much higher with a graphics card than without because graphics require a high load of processing power and memory. Graphics cards take the "expense" of managing graphics on the CPU in integrated systems, and instead have their own CPUs, often known as Graphical Processing Units (GPUs), and their own memory. These standalone systems focus their computational power on the specifics of graphics, including the wire-framing of 3D images into triangles and other polygons, and the management of color, edge smoothing, etc, often requiring speeds of over 25 frames/minute and sometimes exceeding 100 frames/min. Therefore in removing this burden from the CPU and allowing a graphics card to handle the work, 3D performance naturally is much higher in these circumstances. This is accomplished through the use of hundreds of simple cores, and thousands of threads, resulting in massive parallelism in the graphical computations.

4. Suggest the role that graphics cards play in machine learning:

GPUs play an important part in machine learning in that their computational abilities discussed above are ideal for parallel performance between a CPU and GPU in a setup known as a GPGPU (General Purpose Graphical Processing Unit) that is able to handle the high demand of the algorithms and data involved in neural networks.