## Binary, Decimal, and Hex

Complete the following problems:

* Convert `11001111` binary
    to hex:
    __1100(2) = 12(10) = C(16), 1111(2) = 15(10) = F(16) --> 0xCF__ 

    to decimal:
    __11001111(2) = 2^7 + 2^6 + 0^5 + 0^4 + 2^3 + 2^2 + 2^1 + 2^0 = 128 + 64 + 8 + 4 + 2 + 1 = 207__

* Convert `4C` hex
    to binary:
    __0x4C = 01001100(2)__

    to decimal:
    __0x4C = 4 * 16^1 + 12 * 16^0 = 64 + 12 = 76(10)__

* Convert `68` decimal
    to binary:
    __68 = 64 + 4 = 2^6 + 2^2 = 01000100(2)__

    to hex:
    __68 = 64 + 4 = 4 * 16^1 + 4 * 16^0 = 0x44__

## Architecture

One paragraph-ish:

* Explain how the CPU provides concurrency:

__The CPU uses time slices for all processes, which means the CPU gives a certain amount of time to a process, halts that process, then gives CPU time to the next process, halts that process, and on and on. This gives an OS the ability to multitask with just a single CPU (core).__


* Describe assembly language and machine language:

__Assembly language is a CPU specific low-level language that is in a human readable form which is really the machine language in symbolic form. Machine language is the bit code that is directly read by the hardware. Hence it's composed of 0's and 1's.__


* Suggest the role that graphics cards play in machine learning:

__Graphic cards have GPUs (Graphic Processing Units) that are _extremely_ good at math-y problems and considering machine learning involves data sets of incredible sizes, GPUs are very well suited for such applications. Additionally, GPUs are much cheaper and flexible than ASICs while having much greater performance than CPUs.__