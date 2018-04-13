<!-- Answers to the Short Answer Essay Questions go here -->

## 1. Explain how the CPU provides concurrency:
  By using a seperate 'process' or 'thread' to execute programs. Dedicated memory space is allocated for each process so they can maintain it's own 'state' e.g. 'Blocked', 'Ready', 'Running'. This allows the operating system to switch between different tasks and pickup where it left off.

## 2. Describe assembly language and machine language:
  Machine language is in binary and can be executed by the CPU. It is the lowest level of code used by programmers. Any languages that are higher level will typically get compiled down to down to machine code.

  Assembly language is essentially machine code with a layer of abstraction to make it more human readable. It will typically use mnemonics to represent instructions, registers and other resources like memory and peripherals. Assembly code is not directly readable by the CPU. It needs to be converted with a compiler or assembler.


## 3. Why is 3D performance so much higher with a graphics card than without?
  The CPU is not needed to write graphics to the display and so it has a seperate channel dedicated to that, which in turn frees up the CPU to manage other resources. The GPU also has higher bandwidth to access memory.

## 4. Suggest the role that graphics cards play in machine learning:
  GPU's are built to do the same operation on different sets of data more efficiently and compute the same instructions in parallel. They also have a higher bandwidth to retrive from memory. The strengths of GPU's are ideal for executing the type of computations that machine learning employs.
