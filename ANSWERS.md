<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency or parallelism:

    Technically, concurrency and parallelism are two different behaviours. One can occur without the other but it is impossible for a single core CPU to perform parallel computing.
    
    Concurrency occurs when a CPU begins work on another process before a previous process or processes have finished. Due to the speed at which the CPU may handle multiple processes, it can appear that they are being handled simultaneously. However, in any given moment a single core CPU is only ever doing work on one process. It will shift its attention from one process to the other at a rate that is so fast, it can appear to be working on multiple processes at the same time.

    Since a single core in a CPU may only ever work on one process at a given time, parallel computing, or the ability to work on more than one process at a single point in time, may be achieved with multiple cores. A dual core CPU could work on 2 processes at any given time. Likewise a quad core processor could work on up to 4 processes at any given time and so forth. While working in parallel, these multi cores could also work concurrently.

2. Describe assembly language and machine language:

    Machine language is as low level as it gets. A series of zeros and ones representing bits, it is the only language a CPU understands. Assembly language, while still fairly low level, translates those series of bits into human readable instructions. To run on a CPU it will need to be converted into machine language via an assembler.


