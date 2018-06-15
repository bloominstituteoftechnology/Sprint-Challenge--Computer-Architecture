<!-- Answers to the Short Problems go here -->

* Convert `11001111` binary

      to hex: 0xCF

      to decimal: 207


* Convert `4C` hex

      to binary: 0b1001100

      to decimal: 76


* Convert `68` decimal

      to binary: 0b1000100

      to hex: 0x44

<!-- Answers to the Short Answer Essay Questions go here -->

  1. Explain how the CPU provides concurrency:

    Concurrency is when two tasks can start, run, and complete in overlapping time periods. Think multitasking on single-core CPU. // 

  2. Describe assembly language and machine language:

    Assembly language is a "Symbolic encoding" of a CPU's opcodes, designed for humans to interpret and write. It uses mnemonic sequences rather than numeric operations, which machine language uses.

  3. Why is 3D performance so much higher with a graphics card than without?

    Graphics card has its own dedicated memory to processing 3D graphics (rendering), hence the name GPU (Graphic Processing Unit). GPU uses parallel architecuture, as opposed to concurrent architeture that CPU uses, which is better geared towards vector and matrix operations, which are typical calculations done for 3D graphics. It can also assist CPU in some of non-3D graphic calculation, increasing the performance of the computer overall in every way.

  4. Suggest the role that graphics cards play in machine learning:

    GPUs are well-suited to machine learning because, as mentioned above, the GPU carries a parallel architecture, which in turn is beneficial for machine-learning (specifically Deep-learning) since it requires same type of calculation like vector and matrix operations as done in 3D rendering. 