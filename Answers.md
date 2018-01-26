* Convert `11001111` binary
    '0 1 2 3 4 5 6 7 8 9 A B C D E F 10'
    '128, 64, 32, 16, 8, 4, 2, 1'
 
    to hex: 0xCF
     
    to decimal: 207


* Convert `4C` hex

    to binary: 0b01001100

    to decimal: 76


* Convert `68` decimal

    to binary: 0b01000100

    to hex: 0x44


* Explain how the CPU provides concurrency:

  Multithreading allows the cpu to provide concurrency or be able to perform multiple processes and threads at the same time. There are many types of multithreading including the most basic type which is coarse-gramined multi threading, to more advance types including interleved multithreading and simultaneous multithreading. The main idea is that one thread will run until it is blocked by an event that normally would create a stall where it might take at least a few hundred cycles for the data to return. Rather than waiting for the stall to resolve, a multi-thread processor would switch execution to another thread is is actually ready to run. Once the data return from the first thread, the first thread would be placed on ready to run thread. 

* Describe assembly language and machine language:

  Machine code can be thought of as the lowest level representation of a compiled or assembled computer program. They are a set of instructions that are executed directly by the cpu itself with each instruction performing a specifc task such as load, alu operation and jump. Most programs are not written in machine language but often higher level languages including assembly language. Assembly language is still a low level language though it is still considered higher than machine code. While machine code is limited to binary (1s and 0s), assembly language utilizes symbols which represent certain tasks. Assembly languages also requires a utility program known as an assumbler which will convert the code into an executable machine code in a process referred as assembling the source code. 

* Suggest the role that graphics cards play in machine learning:

  GPUs are used to train these deep neural networks using far larger training sets, in an order of magnitude less time, usingfar less datacenter infrastructure.  GPUs are also being used torun these trained machine learning models to do classificationand prediction in the cloud, supporting far more data volume and throughput with less power and infrastructure. All of this is helpful for deep learning and help solve many problems surrounding big data. 
