Convert 1100 1111 binary

to hex: 
CF

to decimal: 1 2 4 8 64 128
207

Convert 4C hex

to binary:
0100 1100

to decimal: 4 8 64
76

Convert 68 decimal

to binary: 0100 0100

to hex: 44

Explain how the CPU provides concurrency or parallelism:
Paralellism is achieved when a cpu breaks a large problem down into smaller subtasks; computes each task simultaneously and then combines subtasks together at the end to produce the final answer. This allows multiple problems to be solved at the same time; rather than having to wait sequentially to solve a list of processes.
Paralellism is achieved through the use of multiple CPU cores and cannot be achieved on a single core processor due to paralellism requiring processes to be completed at the same INSTANT. In concurrency process lifespans can overlap but they do not need to be completed at the same time. However because they are not completed at the same time there needs to be runtime communication between processes to ensure data is transferred between processes correctly resulting in the expected answer.

Describe assembly language and machine language:
Machine language is the binary 1's and 0's a cpu uses to complete processes. Due to it's nature and structure it is not really readable by human standards without breaking down and interpreting it piece by piece.
Assembly language on the other hand is a low level programming language where machine code is broken into parts and on those parts are given small descriptions explaining the meaning and purpose of the proceeding machine code to provide a more human readable structure.