## Binary, Decimal, and Hex

Complete the following problems:

* Convert `11001111` binary

    to hex: parseInt()

    to decimal:  Number('0b11001111');


* Convert `4C` hex

    to binary: 

    to decimal:


* Convert `68` decimal

    to binary: 68.toString(2);

    to hex:    68.toString(16);


## Architecture

One paragraph-ish:

* Explain how the CPU provides concurrency:
The CPU provides concurrency by keeping the state of different running applications in ram and then context switching or loading different programs at different times and running them , thus giving the impression that these programs are running concurrently when in fact only a single one is running at any time. When a program is about to run , the currently running programs state is save and put on a stack , meanwhile the ohter programs stack is read into working register memory and worked on. 

* Describe assembly language and machine language:
A computer at it's most basic level does it work at an electric level, either a current is on  or off . Binary numbers (being base 2) are a good fit for this model. Writing programs in binary was standard 50 years ago but was very error prone and cumbersone as people need a lot of dedication and training to begin to tink more reading and writing binary programs. The next level of abstraction was assembly language which had a dictionary of short instructions that a person could use to construct a program. The assember would translate these instrucitons into machine code before the program could be ran. Today higher level languages such as Javascript (interpreted) and Java (compile to machine language before running)

* Suggest the role that graphics cards play in machine learning:
https://medium.com/ai-society/my-first-experience-with-deep-reinforcement-learning-1743594f0361
Deep reinforcement learning allows neural networks to learn to play games (but not actually understand them) by running them and evaluating which pathways lead to higher scores and thus improving on each interation of game play. This has lead neural networks to best the best go players in the world . This has wider implications in every field of industry and also AI in general. Graphic cards can simulate and render complex computations needed for these neutral networks