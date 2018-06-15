# Project: The LS-8 Emulator

## Module 1

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

* one of the opcode's arguments (e.g. `00000000` for `R0` or `00001000` for the
  value `8`), also known as the _operands_.

This code above requires the implementation of three instructions:

* `LDI`: load "immediate", store a value in a register, or "set this register to
  this value".
* `PRN`: a pseudo-instruction that prints the numeric value stored in a
  register.
* `HLT`: halt the CPU and exit the emulator. Calling `stopClock()` should cause
  it to exit.

See [the LS-8 spec](../../LS8-SPEC.md) for more details.

The above program is already hardcoded into the source file `ls8.js`. To run it,
you will eventually:

```
node ls8
```

but you'll have to implement those three above instructions first!

### Step 1: Implement missing features in `ram.js`

This is the memory store which is just a big array. Two functions exist, one to
read and return a value from an address, and one to write a value to an
address.

The address is just the index into the memory array.

These functions are basically 1-liners.

### Step 2: Implement `tick()`

This is the workhorse function of the entire processor. It's the most difficult
part to write.

It needs to read the memory address that's stored in register `PC`, and store
that result in `IR`, the _Instruction Register_. This can just be a local
variable in `tick()`.

Some instructions requires up to the next two bytes of data _after_ the `PC` in
memory to perform operations on. Sometimes the byte value is a register number,
other times it's a constant value (in the case of `LDI`). Using
`this.ram.read()`, read the bytes at `PC+1` and `PC+2` from RAM into variables
`operandA` and `operandB` in case the instruction needs them.

Then, depending on the value of the opcode, perform the actions needed for the
instruction per the LS-8 spec. Maybe a `switch` statement...? Plenty of options.

After the handler returns, the `PC` needs to be updated to point to the next
instruction for the next iteration of `tick()`. The number of bytes an
instruction uses can be determined from the two high bits (bits 6-7) of the
instruction opcode. See the LS-8 spec for details.

### Step 3: Implement the `HLT` instruction handler

Down in the `HLT()` function, you need to halt the CPU. For now, this just means
calling `stopClock()` to stop the CPU clock. The emulator should exit
automatically once the clock stops.

> It exits because node only runs as long as you're in a function or you have
> timers active.


### Step 4: Add the `LDI` instruction

This instruction sets a specified register to a specified value.

See the LS-8 spec for the details of what this instructions does and its opcode
value.


### Step 5: Add the `PRN` instruction

This is a very similar process to adding `LDI`, but the handler is simpler. See
the LS-8 spec.

*At this point, you should be able to run the program and have it print `8` to
the console!*

### Step 6: Implement a Multiply and Print the Result

Extend your LS8 emulator to support the following program:

```
# mult.ls8

10011001 # LDI R0,8
00000000
00001000
10011001 # LDI R1,9
00000001
00001001
10101010 # MUL R0,R1 <---
00000000
00000001
01000011 # PRN R0
00000000
00000001 # HLT
```

You can change the hard-coded program in `ls8.js` to have the above
instructions. Then running it should produce:

```
72
```

Check the LS-8 spec for what the `MUL` instruction does.

Note that doing the multiply is the responsiblity of the ALU, so it would be
nice if your code eventually called the `alu()` function with appropriate
arguments to get the work done.

### Step 7: Un-hardcode the machine code

In `ls8.js`, the LS-8 programs you've been running so far have been hardcoded
into the source. This isn't particularly user-friendly.

Make changes to `ls8.js` so that the program can be specified on the command
line like so:

```
node ls8 mult.ls8
```

(The programs `print8.ls8` and `mult.ls8` are provided in the source directory
for your convenience.)

For processing the command line, the `process.argv` array holds the
individual words from the command line. For example, if the user runs
`node ls8 mult.ls8`, then the values in the array will be:

```
process.argv[0] === "node"
process.argv[1] === "ls8"
process.argv[2] === "mult.ls8"
```

so you can look in `process.argv[2]` for the name of the file to load.

> Bonus: check to make sure the user has put a command line argument where you
> expect, and print an error and exit if they didn't.

Look at Node's
[fs.readFileSync()](https://nodejs.org/api/fs.html#fs_fs_readfilesync_path_options)
for details on how to read files. Or maybe if you're thinking of reading the
file a line at a time, [this stack overflow answer might
help](https://stackoverflow.com/a/32599033).

As you process lines from the file, you should be on the lookout for blank lines
(ignore them), and you should ignore everything after a `#`, since that's a
comment.

Use the `cpu.poke()` routine to load bytes from the file into memory, similar to
the way it's coded now.

### Step 8: Beautify your `tick()` function, if needed

Do you have a big `if-else-if` block or `switch` block in your `tick()`
function? Is there a way to better modularize your code?

If you haven't done so, consider having independent handler functions, one per
instruction, that does each instruction's work.

Another option is to use something called a _branch table_ to simplify the
instruction handler dispatch code. This is an array of functions that you can
index by opcode value. The upshot is that you fetch the instruction value from
RAM, then use that value to look up the handler function in the branch table.
Then call it.

```js
// Pseudocode, not valid JS:

const LDI = 0b10011001; // From the LS-8 spec
const HLT = 0b00000001;

function handle_LDI() { ... }
function handle_HLT() { ... }

branchTable[LDI] = handle_LDI;
branchTable[HLT] = handle_HLT;

let IR = this.mem.read(this.reg.PC); // Fetch instruction
let handler = branchTable[IR]; // Look up handler in branch table

handler(); // Call it

// etc.
```

## Module 2

###  Implement System Stack

All CPUs manage a _stack_ that can be used to store information temporarily.
This stack resides in main memory and typically starts at the top of memory (at
a high address) and grows _downward_ as things are pushed on. The LS-8 is no
exception to this.

* Implement a system stack per the spec. Add `PUSH` and `POP` instructions. Read
  the beginning of the spec to see which register is the stack pointer, and
  where the stack starts in memory.

### Implement Subroutine Calls

Back in my day, functions were called _subroutines_. In machine code, this
enables you to jump to an address with the `CALL` instruction, and then return
back to where you called from with the `RET` instruction. This enables you to
create reusable functions.

The stack is used to hold the return address, so you **must** implement the
stack, above, first.

* Add subroutine calls. `CALL` and `RET`.

For `CALL`, you will likely have to modify your handler call in `tick()`. The
problem is that some instructions want to execute and move to the next
instruction like normal, but others, like `CALL` and `JMP` want to go to a
specific address.

In **any** case where the instruction handler sets the `PC` directly, you
_don't_ want to advance the PC to the next instruction. So you'll have to set up
a special case for those types of instructions.


## Stretch Goal

Add interrupts to the LS-8 emulator from the Computer Architecture 1.

**For this assignment, keep using the repo you cloned for
Computer-Architecture-One. Don't put it in this repo.**

**You must have implemented a CPU stack before doing this.**

**You must have implmented the `ST` instruction before doing this.**

See the [LS-8
spec](https://github.com/LambdaSchool/Computer-Architecture-One/blob/master/LS8-SPEC.md)
for details on implementation.

The LS-8 should fire a timer interrupt one time per second. This could be
implemented with `setInterval()`, where the handler sets bit 0 of the IS
register (R6). (This is in addition to the normal `tick()` timer you already
have.)

Later in the main instruction loop, you'll check to see if bit 0 of the
IS register is set, and if it is, you'll push the registers on the
stack, look up the interrupt handler address in the interrupt vector
table at address `0xF8`, and set the PC to it. Execution continues in
the interrupt handler.

Then when an `IRET` instruction is found, the registers and PC are
popped off the stack and execution continues normally.

## Example

This code prints out the letter `A` from the timer interrupt handler
that fires once per second.

```
# interrupts.ls8

10011001 # LDI R0,0xF8
00000000
11111000
10011001 # LDI R1,INTHANDLER
00000001
00010001
10011010 # ST R0,R1  Set the interrupt vector at 0xF8 to INTHANDLER
00000000
00000001
10011001 # LDI R5,1  Set the IM register so we receive timer interrupts
00000101
00000001
10011001 # LDI R0,LOOP  Infinite loop forever
00000000
00001111
# LOOP (15):
01010000 # JMP R0
00000000

# Timer interrupt Handler
# When the timer interrupt occurs, output an 'A'
# INTHANDLER (17):
10011001 # LDI R0,65   A is 65 in ASCII
00000000
01000001
01000010 # PRA R0      Print it
00000000
00001011 # IRET        Return from interrupt
```


The assembly program is interested in getting timer interrupts, so it sets the
IM register to `00000001` with `LDI R5,1`.

The interrupt `setInterval()` timer fires and sets bit #0 in IS.

At the beginning of `tick()`, the CPU checks to see if interrupts are enabled.
If not, it continues processing instructions as normal. Otherwise:

Bitwise-AND the IM register with the IS register. This masks out all the
interrupts we're not interested in, leaving the ones we are interested in:

```javascript
let interrupts = this.reg[IM] & this.reg[IS];
```

Step through each bit of `interrupts` and see which interrupts are set.

```javascript
for (let i = 0; i < 8; i++) {
  // Right shift interrupts down by i, then mask with 1 to see if that bit was set
  let interruptHappened = ((interrupts >> i) & 1) == 1;

  ...
}
```

(If the no interrupt bits are set, then stop processing interrupts and continue
executing the current instruction as per usual.)

If `interruptHappened`, check the LS-8 spec for details on what to do.


