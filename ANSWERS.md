<!-- Answers to the Short Answer Essay Questions go here -->

1.  Explain how the CPU provides concurrency:
    The way it's laid out it needs to provide that to work with slow user input and things like
    hard drive reads. Interrupts are one way the CPU provides this, another is using multiple units
    to compartmentalize reads.

2.  Describe assembly language and machine language:
    Machine language is broken into actual 1's and 0's to be read verbatim by the machine,
    Whereas assembly language is very slightly above that in that it's somewhat readable by
    humans with instructions not yet converted to numbers.

3.  Why is 3D performance so much higher with a graphics card than without?
    Because rendering graphics is massively parallel work and graphics cards have thousands of
    "cores" each given a piece of work to perform and they are tailor made to work on graphical representation.

4.  Suggest the role that graphics cards play in machine learning:
    Machine learning is similar to graphics work in that it is also able to be massively parallelized -
    each "core" can act as a virtual neuron that the machine can connect with the others and learn with.
