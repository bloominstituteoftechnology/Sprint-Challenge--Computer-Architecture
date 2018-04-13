<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency:
- For single-core CPU's, the CPU can keep track of several running processes, and initialize them in sequence, and by priority, so they can perform their tasks without overlapping their executions, providing their results to the CPU asynchronously (thus preventing blocking). Additionally, some work is performed by other components, such as an arithmetic logic unit, and the results provided to the CPU as needed, which frees the CPU from needing to perform that work. In multi-core systems, processes can also be executed in parallel, though they still make use of concurrency methods as described above.


2. Describe assembly language and machine language:
- Machine language is the raw binary instructions built into, and understood, by a CPU.
- Assembly language is basically the human-readable version of machine language, with a 1-to-1 conversion between assembly instructions to binary on compile.


3. Why is 3D performance so much higher with a graphics card than without?
- Graphics cards carry their own dedicated processors (GPU) for handling instructions relevant to image rendering, taking this workload off of the CPU.


4. Suggest the role that graphics cards play in machine learning:
- GPU's have far more processing cores than CPU's (thousands versus dozens), each one being simpler than what would be found in a CPU, allowing them to process many simple instructions in parallel, versus a CPU processing fewer, more complex instructions in parallel. The end result is that the same instructions sent to a GPU will be processed more efficiently than those sent to a CPU, and this processing power can be further increased by running multiple GPU's in parallel.