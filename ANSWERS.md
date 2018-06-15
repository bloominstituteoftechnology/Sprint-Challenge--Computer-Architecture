## Binary, Decimal, and Hex

Complete the following problems:

* Convert `11001111` binary 

    to hex:
    207 => hex, F = 15, which is the max, then 10 hex starts 16 dec
    XY = X*(16^1) + Y = 207; X & Y maximum = F = 15;
    CF = 12 * (16^1) + 15 = 12 * 16 + 15 = 144 + 48 + 15 = 192 + 15 = 207
    Where 12 = C; and 15 = F
    So, `11001111` = CF;
    to decimal:
    128 + 64 + 0 + 0 8 + 4 + 2 + 1 = 207 decimal


* Convert `4C` hex

    to binary:
    4 * (16^1) + 12 = 64 + 12 = 76 decimal
    01001100 = 0 + 64 + 0 + 0 + 8 + 4 + 0 + 0 = 64 + 8 + 4 = 76
    to decimal:
    4 * (16^1) + 12 = 64 + 12 = 76 decimal
* Convert `68` decimal

    to binary:
    01000100 = 0 + 64 + 0 + 0 + 0 + 4 + 0 + 0 = 68;
    to hex:
    XY = X*(16^1) + Y = 68; X & Y maximum = F = 15;
    44 (hex) = 4 * (16^1) + 4 = 64 + 4 = 68 decimal
<!-- Answers to the Short Answer Essay Questions go here -->

1. Explain how the CPU provides concurrency:
It's able to provide concurrency because it keeps track of the point counter through the use of the stack or the register. When an interrupt occurs, the stack is used to keep track of the point counter, and after the interrupt is completed, the processor continues to work on the point counter that it left. The point counter (the current instruction) is kept track of by the instructions that either save it onto the stack or register. Overall, since the tracker is saved, the CPU provides concurrency.
<!-- Graphic cards were NOT COVERED in lecture -->
2. Describe assembly language and machine language:
They're bytes of information, and the value of the bytes are read and interpreted as instructions corresponding to the different cases.
<!-- Graphic cards were NOT COVERED in lecture -->
3. Why is 3D performance so much higher with a graphics card than without?
Graphics requires a lot of parallel computation, which graphics cards do very efficiently. CPUs are for useful for a lot of things, other than parallel computation, but it can still do parallel computations, but not as efficient. So, CPUs can do a lot of different computations, but they're not designed to be as efficient as GPUs in graphics.

<!-- Graphic cards were NOT COVERED in lecture -->
4. Suggest the role that graphics cards play in machine learning:
Graphic cards are a lot more efficient at data computations, and they have a lot more cores than CPUs. CPUS are designed to have a wide array of things that it can handle, while GPUs are not, GPUs are designed to be very efficient at parallel computations (all the cores are used efficiently).