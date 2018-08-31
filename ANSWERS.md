<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency:
   A: Concurrent processing is a computing model in which multiple processors execute instructions simultaneously for better performance (instead of sequentially). Concurrent processes can be executed on one core by interleaving the execution steps of each process via time-sharing slices: only one process runs at a time, and if it does not complete during its time slice, it is paused, another process begins or resumes, and then later the original process is resumed.

2. Describe assembly language and machine language:
   A: Assembly language is a more human readable view of machine language. Instead of representing the machine language as numbers, the instructions and registers are given names (typically abbreviated words, or mnemonics, eg ld means "load"). Unlike a high level language, assembly is very close to the machine language.
