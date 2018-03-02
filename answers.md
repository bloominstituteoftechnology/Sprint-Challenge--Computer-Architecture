## Binary, Decimal, and Hex

Complete the following problems:

* Convert `11001111` binary

    to hex: cf

    to decimal: 207


* Convert `4C` hex

    to binary:1001100

    to decimal:76


* Convert `68` decimal

    to binary: 1000100

    to hex: 44


## Architecture

One paragraph-ish:

* Explain how the CPU provides concurrency:

    Concurrency is described as in computer terms as completing things out of order or in a partial order; while not affecting the final outcome.
    A CPU leverages internal memory caches as well as all the the components at it's disposal to off load tasks to each respective component required to do a specific task. While each component is completed their respective tasks, the CPU is completing the bridges and  once each component flags the CPU that they are done, the CPU then compiles all the data performs the necessary computations and sends the end result to the proposed final destination.

* Describe assembly language and machine language:
Assembly language is the use of mnemonics and special syntax to represent low-level computer operations. It frees a CPU from having to do repetitive calculations by simplifying the machine code prior to feeding it into the CPU for computation

Machine language is a programming language that written in binary and hexadecimal, used to communicate directly with a cpu.

* Suggest the role that graphics cards play in machine learning:
GPU's are designed specifically for performing calculations, unlike a CPU that has to split it's computing power amongst all computer operations from connecting peripherals, interfacing with networks, to producing audio.

GPU's are not burdened by the totality of computer operation and can be hyper focused on data computations. Since GPU's are not concerned with other operations they are smaller than CPU's; therefore more GPU's can be packed into a small space for concurrent data computation.
