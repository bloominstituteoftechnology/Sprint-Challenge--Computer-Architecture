<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency or parallelism:

Concurrency has to do with managing access to shared state from difference threads.  A thread can be thought of as a series of instructions, which can then be collected into a larger process.  We saw examples of these threads in our computer architecture project which has us running instructions in sequence in order to perform some overall operation like print 8.  For this thread we need an id “print8”, a program counter, “PC”, and a collection of registers.  Some of the threads we worked with also relied upon a stack in order to improve and simplify our access to the state.  So as I understand it, “concurrency” is about sharing the things that the threads rely upon in order to function, such as blocks of code, data, and the registers which can be used for different purposes depending on the goal of the thread. 
Parallelism is a concept distinct from concurrency and is about improving performance by running multiple processors at the same time (in parallel).  Processes combine multiple threads in order to perform a job.  It would not be efficient for threads to run one at a time.  An operating system will allocated resources in a way that will prevent threads from interfering with one another so that they can in fact run side by side.  

I did some research here:
https://learn-gevent-socketio.readthedocs.io/en/latest/general_concepts.html

2. Describe assembly language and machine language:

Machine language can be thought of as the only language a computer can understand.  
It consists almost entirely of numbers, often binary, which makes it nearly impossible for a human to read and interpret.  Machine language is only one step away from the computer hardware and consists of instructions telling the hardware what to do.  Human’s have developed what we refer to as high level programming languages, which are more efficient for us to read, write and understand.  In between the high level programming languages (like C, Python, Javascript) and machine language is the assembly language.  Assembly  language is a sort a compromise between high level programming languages and machine language.  With the assembly language, we build a series of instructions in blocks that make sense to us, but connect our instructions to the machine language.  This code is then compiled into machine language so that the computer will know what to do.  What I have come to understand this week is the hierarchy within the computing world which we might say starts at User Interface as the highest level and works its way down to the lowest level being the actual machine.  Until this week, I wasn’t totally clear on what high level and low level actually meant, but this concept is very clear to me now. 

Computing Hierarchy:

UI > Python, C, JS etc. > Assembly > Machine Language

* Convert `11001111` binary

    to hex:
        1100 is C
        1111 is F
        11001111 is CF

    to decimal:
        128+64+0+0+8+4+2+1 = 207


* Convert `4C` hex

    to binary: 
        4 is 100
        C is 1100
        4C is 01001100

    to decimal:
        0+64+0+0+8+4+0+0 = 76


* Convert `68` decimal

    to binary:
        01000100

    to hex:
        0100 is 4
        0100 and 0100 is 44