# Project: The LS-8 Emulator

## Day 1

### Implementation of the LS-8 Emulator

Objective: to gain a deeper understanding of how a CPU functions at a
low level.

We're going to write an emulator for the world-famous LambdaSchool-8 computer,
otherwise known as LS-8! This is an 8-bit computer with 8-bit memory addressing,
which is about as simple as it gets.

An 8 bit CPU is one that only has 8 wires available for addresses (specifying
where something is in memory), computations, and instructions. With 8 bits, our
CPU has a total of 256 bytes of memory and can only compute values up to 255.
The CPU could support 256 instructions, as well, but we won't need them.

We'll design a memory subsystem, an ALU, and the main CPU emulator code
in JS. Then we'll write code that loads a file of machine code
instructions from disk, parses the file, and loads it into memory. After
that, we'll start the emulator running and see it execute instructions!

For starters, we'll execute code that stores the value 8 in a register,
then prints it out:

```
# print8.ls8

10011001 # LDI R0,8  Store 8 into R0
00000000
00001000
01000011 # PRN R0    Print the value in R0
00000000
00000001 # HLT       Halt and quit
```

The binary numeric value on the left in the `print8.ls8` code above is either:

* the machine code value of the instruction (e.g. `10011001` for `LDI`), also
  known as the _opcode_

or

* one of the opcode's arguments (e.g. `00000000` for R0 or `00001000` for the
  value `8`), also known as the _operands_.

This code above requires the implementation of three instructions:

* `LDI`: load "immediate", store a value in a register
* `PRN`: a pseudo-instruction that prints the numeric value stored in a register
* `HLT`: halt the CPU and exit the emulator

See [the LS-8 spec](../../LS8-SPEC.md) for more details.

To run the above program, you use the command line:

```
node ls8 print8.ls8
```

### Step 1: Read the input file

In `ls8.js`, there is a `processFile()` function. This needs to be implemented.
It will process the input file (similar to above) one line at a time, store the
value of the binary number in memory using the `.poke()` function on the `cpu`.

The first number should be loaded at address 0, the second at address 1, and so
on.

At this point, study the rest of `ls8.js` to see how it works.

### Step 2: Implement missing features in `ram.js`

This is the memory store which is just a big array. Two functions exist, one to
read and return a value from and address, and one to write a value to an
address.

The address is just the index into the memory array.

These functions are basically 1-liners.

### Step 3: Implement `tick()`

This is the workhorse function of the entire processor. It's the most difficult
part to write.

It needs to read the memory address that's stored in register `PC`, and store
that result in register `IR`.

Then it looks up the handler in the `branchTable` based on the value in `IR`.

If the handler doesn't exist, it needs to exit the emulator with an "unknown
instruction" error.

Some instructions requires up to the next two bytes of data _after_ the `PC` in
memory to perform operations on. Sometimes the byte value is a register number,
other times it's a constant value (in the case of `LDI`). Using
`this.ram.read()`, read the bytes at `PC+1` and `PC+2` from RAM into variables
`operandA` and `operandB` to pass into the handler function.

It then can call the handler, passing in `operandA` and `operandB`. Some
handlers don't use those arguments, so they will be ignored in those cases.

After the handler returns, the `PC` needs to be updated to point to the next
instruction for the next iteration of `tick()`. The number of bytes an
instruction uses can be determined from the two high bits (bits 6-7) of the
instruction opcode. See the LS-8 spec for details.

### Step 4: Implement the `HLT` instruction handler

Down in the `HLT()` function, you need to halt the CPU. For now, this just means
calling `stopClock()` to stop the CPU clock. The emulator should exit
automatically once the clock stops.

> It exits because node only runs as long as you're in a function or you have
> timers active.

At this point, you should be able to run this program, which has no output and
simply exits silently:

```
# halt.ls8
00000001 # HLT
```

### Step 5: Add the LDI instruction

To add this, the `LDI` must be added to the `branchTable` similarly to `HLT`.

Then the `LDI()` handler function must be created. It will take two parameters,
one of which is a register number to store a value in, and the second is the
value to store.

See the LS-8 spec for the details of what this instructions does.


### Step 6: Add the `PRN` instruction

This is a very similar process to adding `LDI`, but the handler is simpler. See
the LS-8 spec.

### Step 7: Implement a Multiply and Print the Result

Extend your LS8 emulator to support the following program:

```
# mult.ls8

10011001 # LDI R0,8
00000000
00001000
10011001 # LDI R1,9
00000001
00001001
10101010 # MUL R0,R1
00000000
00000001
01000011 # PRN R0
00000000
00000001 # HLT
```

The following command line input:

```
node ls8 mult.ls8
```

Should produce

```
72
```

through the process of executing the machine code in the input file.


## Day 2

###  Implement System Stack

All CPUs manage a _stack_ that can be used to store information
temporarily. This stack resides in main memory and typically starts at
the top of memory (at a high address) and grows _downward_ as things are
pushed on. The LS-8 is no exception to this.

* Implement a system stack per the spec. Add `PUSH` and `POP`
  instructions. Read the beginning of the spec to see which register is
  the stack pointer, and where the stack starts in memory.

### Implement Subroutine Calls

Back in my day, functions were called _subroutines_. In machine code,
this enables you to jump to an address with the `CALL` instruction, and
then return back to where you called from with the `RET` instruction.
This enables you to create reusable functions.

The stack is used to hold the return address, so you **must** implement
the stack, above, first.

* Add subroutine calls. `CALL` and `RET`.

For `CALL`, you will likely have to modify your handler call in `tick()`. The
problem is that some instructions want to execute and move to the next
instruction like normal, but others, like `CALL` and `JMP` want to go to a
specific address.

A modification you can make is allow handlers to optionally return a value. If
the value is `undefined`, then you move the `PC` to the next instruction like
normal. But if the value is a number, then you move the `PC` directly to that
address.