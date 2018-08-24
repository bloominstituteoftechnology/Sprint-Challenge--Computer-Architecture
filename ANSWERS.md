<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency:
CPU doesn't have true concurrency.  It can make it seem like it's concurrent though.  
Basically CPU's are really fast and do billions of calculations/second.  
When an interrupt if given it pauses what it's doing and does the interrupt task first.
Then it resumes the task it was doing beforehand.
There's other methods, such as hyper threading and multicores, but interrupt is what we learned this week.

2. Describe assembly language and machine language:
    assembly language is something humans can read like hex? It's very line to line instructions
    It's usually specific to a computer/machine and has to be compiled.
    machine lanugage is binary code that machines can read


