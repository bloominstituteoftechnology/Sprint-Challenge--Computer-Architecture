# Answers

### Conversion

* Convert `11001111` binary  

    to hex: `parseInt(11001111, 2).toString(16)` => **CF**  
    to decimal: `parseInt(11001111, 2);` => **207**


* Convert `4C` hex  

    to binary: `parseInt('4C', 16).toString(2)` => **1001100**  
    to decimal: `parseInt('4C', 16)` => **76**


* Convert `68` decimal  

    to binary: `(68).toString(2)` => **1000100**  
    to hex: `(68).toString(16)` => **44**

---

## Architecture

One paragraph-ish:

* **Explain how the CPU provides concurrency**:  
*Answer*: Concurrency in processing is when two or more processes/task can be run at the same time. Nowadays, processors (CPU) comes with multiple cores (physical and virtual). Each core can process instructions in parallel with other cores to complete the given task.

* **Describe assembly language and machine language**:  
*Answer*: Assembly Language is a low-level programming language which is converted to machine executable code. It is the human-readable form of machine Language. Machine Language consists of instructions to the processor in binary/hex form.


* **Suggest the role that graphics cards play in machine learning**:  
*Answer*: Graphic Cards (alias GPU) unlike CPU's have more number of cores, which accelerates the performance of a computational task. Multiple cores means multiple processes can be run concurrently. Therefore, in machine learning multiple learning algorithms can be run at the same time using GPUs.