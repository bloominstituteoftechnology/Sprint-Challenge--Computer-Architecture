<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency:
  * The CPU uses delegation and memory caching to provide concurrency. Basically, the CPU runs on a cycle, and decides what to do each iteration based on flags and interrupts. The CPU can assign a task to a component and expect a respective flag to be triggered when that task is finished. Then, it can store everything it knows about itself in RAM, do what it needs to do based on the given flag(s), reload itself from RAM, and continue its normal cycle.

----

2. Describe assembly language and machine language:
  * Assembly language is low-level (meaning more fundamental) code that is meant to be directly read and manipulated by humans. It usually consists of things like mnemonics, operands, and comments. It requires a compiler to be transferred into machine language.

  * Machine language is as close to communicating directly with a computer as one can get without just transmitting electric signals like a weirdo. It is comprised of numbers, usually binary or hex. A computer directly interprets these numbers as instructions for what it should do.

  ----


3. Why is 3D performance so much higher with a graphics card than without?
  * Rendering a 3D environment involves the transfer/manipulation of a lot of data. A graphics card meets this challenge with its GPU, which is basically a CPU designed to maximize quantity instead of speed. A graphics card's ability to handle large amounts of data in parallel (at the same time) results in an increased performance in rendering 3D videos/images.

  ---


4. Suggest the role that graphics cards play in machine learning:
  * Machine learning requires a lot of training/testing to get the machine where it needs to be. The potential role of graphics cards becomes a choice between running a few training sessions at once, but quickly VS running many training sessions at once, but less quickly. A graphics card allows multiple (and many) instances of a given ML algorithm to run at the same time, resulting in more training/testing reps for the machine.
