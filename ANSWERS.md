<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency:

    The CPU provides concurrency using interrupts and stacks.

2. Describe assembly language and machine language:

    Assembly language is a step lower than higher-level languages like Javascript but still human readable while machine language is a series of 1s and 0s which represent instructions for the computer to perform. 

3. Why is 3D performance so much higher with a graphics card than without?

    This is because a graphics card along with its GPU are specialized processors that handle complicated calculations related to 2D and 3D graphics compared to the central processor.


4. Suggest the role that graphics cards play in machine learning:
    A GPU core is optimized exclusively for data computations and thus ML applications, which perform large numbers of computations on a vast amount of data, can see huge performance improvements when running on a GPU versus a CPU.

## Binary, Decimal, and Hex

Complete the following problems:

* Convert `11001111` binary

    to hex: parseInt('11001111', 2).toString(16) // CF

    to decimal: parseInt('11001111', 2) // 207


* Convert `4C` hex

    to binary: parseInt('4C', 16).toString(2) // 01001100

    to decimal: parseInt('4C', 16) // 76


* Convert `68` decimal

    to binary: 
    
    let val = 68;
    val.toString(2); // 0100 0100

    to hex:

    let val = 68;
    val.toString(16); // 44