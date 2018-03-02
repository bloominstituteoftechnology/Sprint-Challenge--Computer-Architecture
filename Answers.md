## Binary, Decimal, and Hex
#### Convert '11001111' binary
* to hex: **CF**
* to decimal: **207**
#### Convert '4c' hex
* to binary: **0b01001100**
* to decimal: **76**
#### Convert '68' decimal
* to binary: **0b01000100**
* to hex: **0x44**
## Architecture
#### Explain how the CPU provides concurrency:
Concurrency means completing tasks out of order without affecting the final result - the CPU does this by using internal memory caches and delegating tasks to different components. Each component completes its assigned tasks and then flags the CPU , the CPU ultimately compiles all the data from the different components and delivers final result.
#### Describe assembly language and machine language:
* __assembly language__: Low level language, more readable than machine language because it employs mnemonics. This keeps the machine from having to waste resources doing repetitive calculations because it's already translated before feeding to the CPU.
* __machine language__: Lowest level language, written in binary and hex, used to communicate with the CPU
#### Suggest the role that graphics cards play in machine learning:
GPUs can take on a lot of parallel processes, taking the burden off CPUs. Computing farms are also loads more expensive than employing a bunch of GPUs. 