# Binary, Decimal and Hex

- Convert `11001111` binary

  to hex: `CF`

  to decimal: `207`

- Convert `4C` hex

  to binary: `1001100`

  to decimal: `76`

- Convert `68` decimal

  to binary: `1000100`
  
  to hex: `44`
  
# Architecture
<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency:

The CPU provides concurrency by performing multiple operations simultaneously. A CPU can have multiple pipelines that operate concurrently. For example, one might load from memory while another might be performing a calcuation.

2. Describe assembly language and machine language:

Assembly language is a more human readable view of machine language. Instead of the language being represented as numbers, the instructions and registers are given names. An assembly language program is eventually translated to machine language by an assembler.

Machine language is the actual bits used to control the processor in a computer. The processor reads the bits in from program memory and the bits represent instructions as to what should happen next.

3. Why is 3D performance so much higher with a graphics card than without? 

3D performance is so much higher with a graphics card than without because they provide dedicated graphics pipelines. This allows graphics memory to be written without needing to be circled around with the CPU.


4. Suggest the role that graphics cards play in machine learning:

GPUs are used in machine learning GPUs offer many benefits over the CPU in performing calculations. Some of these include the fact that a GPU has hundreds of simple cores over the CPU's fex but complex cores, and a GPU has thousnads of concurrent hardware threads while the CPU has single-thread performance optimization. These differences allow the GPU to perform many operations at the same time instead of doing it one after another which means that training a neural network can be done much faster with the GPU.