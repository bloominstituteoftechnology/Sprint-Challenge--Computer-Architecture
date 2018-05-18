<!-- Answers to the Short Answer Essay Questions go here -->

#

1.Explain how the CPU provides concurrency:

* Concurrency is concerned with managing access to shared state from different threads, whereas parallelism is concerned with utilizing multiple processors/cores to improve the performance of a computation.

2.Describe assembly language and machine language:

* Assembly language is more human readable which uses mnmonic sequences instead of numeric operation codes used in machine language.

3.Why is 3D performance so much higher with a graphics card than without?

* A graphics card has a sophisticated computer chip, with thousands of concurrent hardware threads, designed explicitly for the task of processing video, the GPU, but it also has dedicated RAM for the task. This increase in power makes 3D performance higher.

4.Suggest the role that graphics cards play in machine learning:

* While a CPU core is more powerful than a GPU core, the vast majority of this power goes unused by ML applications. A CPU core is designed to support an extremely broad variety of tasks in addition to performing computations, whereas a GPU core is optimized exclusively for data computations. Because of this singular focus, a GPU core is simpler and has a smaller die area than a CPU, allowing many more GPU cores to be crammed onto a single chip. Consequently, ML applications, which perform large numbers of computations on a vast amount of data, can see huge performance improvements when running on a GPU versus a CPU.