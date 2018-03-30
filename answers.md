# Link to my code PR:
https://github.com/LambdaSchool/Computer-Architecture-One/pull/72



# Sprint-Challenge--Computer-Architecture

## Binary, Decimal, and Hex

Complete the following problems:

* Convert `11001111` binary

    to hex: CF

    to decimal: 207


* Convert `4C` hex

    to binary: 01001100

    to decimal: 76


* Convert `68` decimal

    to binary: 01000100

    to hex: 44


## Architecture

One paragraph-ish:

* Explain how the CPU provides concurrency:

I'm not exactly sure about this - I did some reading on concurrent vs parallel computing and my understanding is that concurrent computing is when a processor is switching between multiple tasks to advance each a bit before switching to advance another, and so on. As to how I think this would be handled is through interrupts or a system like that where multiple programs may be running but the cpu is only actively handling any one program at a time. The states of the others are stored when they are not being executed.

In effect, programs A, B, and C are loaded. A is running first with B and C states saved. A bit of A is handled, then program A state is saved and program B state is loaded, a bit of B runs, then B state is saved and C state is loaded, a bit of C runs, then C state is saved and A state is loaded, etc.

* Describe assembly language and machine language:

Assembly language is human readable, and it is turned into machine language with the use of a compiler. Assembly language cannot be executed by the computer; it must be compiled into machine language, which the computer can execute. Assembly uses human-readable code comprised of mnemonics, operands, labels, and comments, while machine language is comprised of zeros and ones.


* Suggest the role that graphics cards play in machine learning:

The use of graphics cards has advanced machine learning greatly by increasing the number of operations that can be done at the same time. GPUs have a high number of cores, can handle thousands of concurrent hardware threads, and maximize floating point throughput. The analogy I read compared a CPU and GPU to a Ferrari and a truck. The Ferrari (CPU) is great for moving a small amount of things quickly, but the truck (GPU) moves much more even if it's at a slower pace. In the same amount of time, the truck (GPU) can haul a lot more than the Ferrari (CPU) can.
