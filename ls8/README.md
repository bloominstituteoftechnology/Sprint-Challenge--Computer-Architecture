# Project: The LS-8 Emulator

## Implementation of the LS-8 Emulator

_Objective_: to gain a deeper understanding of how a CPU functions at a
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
# print8.ls8: Print the number 8 on the screen

10000010 # LDI R0,8
00000000
00001000
01000111 # PRN R0
00000000
00000001 # HLT
```

The binary numeric value on the left in the `print8.ls8` code above is either:

* the machine code value of the instruction (e.g. `10000010` for `LDI`), also
  known as the _opcode_

or

* one of the opcode's arguments (e.g. `00000000` for `R0` or `00001000` for the
  value `8`), also known as the _operands_.

This code above requires the implementation of three instructions:

* `LDI`: load "immediate", store a value in a register, or "set this register to
  this value".
* `PRN`: a pseudo-instruction that prints the numeric value stored in a
  register.
* `HLT`: halt the CPU and exit the emulator.

See [the LS-8 spec](../LS8-spec.md) for more details.

The above program is already hardcoded into the source file `cpu.c`. To run it,
you will eventually:

```
make
./ls8
```

but you'll have to implement those three above instructions first!

## Step 0: IMPORTANT: inventory what is here!

* Make a list of files here.
* Write a short 3-10-word description of what each file does.
* Note what has been implemented, and what hasn't.
* Read this whole file.
* Skim the spec.

## Step 1: Implement `struct cpu` in `cpu.h`

This structure holds information about the CPU and associated components.

The type for a single unsigned byte in C is:

```c
unsigned char x;
```

(Weirdly in C, if you don't specific `signed` or `unsigned` with a `char`, it's
up to the compiler which it uses.)

## Step 2: Add RAM functions

In `cpu.c`, add functions `cpu_ram_read()` and `cpu_ram_write()` that access the
RAM inside the `struct cpu`.

We'll make use of these helper function later.

## Step 3: Implement the core of `cpu_init()`

The `cpu_init()` function takes a pointer to a `struct cpu` and initializes it
as necessary. At first, the PC, registers, and RAM should be cleared to zero.
(`memset()` might help you clear registers and RAM.)

Later on, you might do further initialization here, e.g. setting the initial
value of the stack pointer.

## Step 4: Implement the core of `cpu_run()`

This is the workhorse function of the entire processor. It's the most difficult
part to write.

It needs to read the memory address that's stored in register `PC`, and store
that result in `IR`, the _Instruction Register_. This can just be a local
variable in `cpu_run()`.

Some instructions requires up to the next two bytes of data _after_ the `PC` in
memory to perform operations on. Sometimes the byte value is a register number,
other times it's a constant value (in the case of `LDI`). Using
`cpu_ram_read()`, read the bytes at `PC+1` and `PC+2` from RAM into variables
`operandA` and `operandB` in case the instruction needs them.

Then, depending on the value of the opcode, perform the actions needed for the
instruction per the LS-8 spec. Maybe a `switch` statement...? Plenty of options.

After the handler returns, the `PC` needs to be updated to point to the next
instruction for the next iteration of the loop in `cpu_run()`. The number of
bytes an instruction uses can be determined from the two high bits (bits 6-7) of
the instruction opcode. See the LS-8 spec for details.

## Step 5: Implement the `HLT` instruction handler

Add the `HLT` instruction to `cpu.h`.

In `cpu_run()` in your switch, exit the loop if a `HLT` instruction is
encountered, regardless of whether or not there are more lines of code in the LS-8 program you loaded. 

We can consider `HLT` to be similar to a `return` or `exit()` in that we stop whatever we are doing, wherever we are.

## Step 6: Add the `LDI` instruction

This instruction sets a specified register to a specified value.

See the LS-8 spec for the details of what this instructions does and its opcode
value.

## Step 7: Add the `PRN` instruction

This is a very similar process to adding `LDI`, but the handler is simpler. See
the LS-8 spec.

*At this point, you should be able to run the program and have it print `8` to
the console!*

## Step 8: Un-hardcode the machine code

In `cpu.c`, the LS-8 programs you've been running so far have been hardcoded
into the source. This isn't particularly user-friendly.

Make changes to `cpu.c` and `ls8.c` so that the program can be specified on the
command line like so:

```
./ls8 examples/mult.ls8
```

(The programs `print8.ls8` and `mult.ls8` are provided in the `examples/`
directory for your convenience.)

For processing the command line, the signature of `main()` should be changed to:

```c
int main(int argc, char *argv[])
```

`argc` is the argument count, and `argv` is an array of strings that hold the
individual arguments, starting with the command name itself.

If the user runs `./ls8 examples/mult.ls8`, the values in `argv` will be:

```c
argv[0] == "./ls8"
argv[1] == "examples/mult.ls8"
```

so you can look in `argv[1]` for the name of the file to load.

> Bonus: check to make sure the user has put a command line argument where you
> expect, and print an error and exit if they didn't.

In `load_cpu()`, you will now want to use those command line arguments to open a file, read in its contents line by line, and save appropriate data into RAM. 

As you process lines from the file, you should be on the lookout for blank lines
(ignore them), and you should ignore everything after a `#`, since that's a
comment.

You'll have to convert the binary strings to integer values to store in RAM. The
built-in `strtoul()` library function might help you here.

## Step 9: Implement a Multiply and Print the Result

Extend your LS8 emulator to support the following program:

```
# mult.ls8: Multiply 8x9 and print 72

10000010 # LDI R0,8
00000000
00001000
10000010 # LDI R1,9
00000001
00001001
10100010 # MUL R0,R1
00000000
00000001
01000111 # PRN R0
00000000
00000001 # HLT
```

One you run it with `./ls8 examples/mult.ls8`, you should see:

```
72
```

Check the LS-8 spec for what the `MUL` instruction does.

> Note: `MUL` is the responsiblity of the ALU, so it would be
nice if your code eventually called the `alu()` function with appropriate
arguments to get the work done.
> 


## Step 10: Beautify your `cpu_run()` loop, if needed

Do you have a big `if-else-if` block or `switch` block in your `cpu_run()`
function? Is there a way to better modularize your code?

If you haven't done so, consider having independent handler functions, one per
instruction, that does each instruction's work.

Another option is to use something called a _branch table_ or _dispatch table_
to simplify the instruction handler dispatch code. This is an array of functions
that you can index by opcode value. The upshot is that you fetch the instruction
value from RAM, then use that value to look up the handler function in the
branch table. Then call it.

```js
// !PSEUDOCODE!

const LDI = 0b10011001; // From the LS-8 spec
const HLT = 0b00000001;

function handle_LDI() { ... }
function handle_HLT() { ... }

branchTable[LDI] = handle_LDI;
branchTable[HLT] = handle_HLT;

let IR = ram_read(this.reg.PC); // Fetch instruction
let handler = branchTable[IR]; // Look up handler in branch table

handler(); // Call it

// etc.
```

This solution involves _pointers to functions_. This is something you've already
likely used for callbacks in other languages, but in C, the syntax is somewhat
**insane**.

Some examples in C:

```c
// Normal function to take two floats and return an int
int foo(float x, float y)
{
  return x * y;
}

int main(void)
{
  // Declare fp to be a pointer to a function that takes two floats and
  // returns an int:

  int (*fp)(float, float); // points at garbage until initialized

  // Initialize fp to point to function foo()
  fp = foo;

  // Now you can call foo() like this:
  int x = fp(2.0, 4.5);

  // Or normally like this:
  int y = foo(2.0, 4.5);

  return 0;
}
```

Arrays of pointers to functions are even zanier:

```c
// Declare an array, bar, of 10 pointers to functions that take an int and
// a float, and return void:

void (*bar[10])(int, float); // Array as yet uninitialized!

// Do the same with an array baz, but also init all the pointers to NULL:

void (*baz[10])(int, float) = {0};
```

Whether you do a `switch` or a branch table or anything else is up to you.

## Step 11: Implement System Stack

All CPUs manage a _stack_ that can be used to store information temporarily.
This stack resides in main memory and typically starts at the top of memory (at
a high address) and grows _downward_ as things are pushed on. The LS-8 is no
exception to this.

Implement a system stack per the spec. Add `PUSH` and `POP` instructions. Read
  the beginning of the spec to see which register is the stack pointer. 
  
* Values themselves should be saved in the ***portion of RAM*** _that is allocated for the stack_. 
  -  Use the stack pointer to modify the correct block of memory. 
  - Make sure you update the stack pointer appropriately as you `PUSH` and `POP` items to and from the stack.

If you run `./ls8 examples/stack.ls8` you should see the output:

```
2
4
1
```

## Step 12: Implement Subroutine Calls

Back in the old days, functions were called _subroutines_. In machine code, subroutines
enable you to jump to any address with the `CALL` instruction, and then return
back to where you called from with the `RET` instruction. This enables you to
create reusable functions.

Subroutines have many similarities to functions in higher-level languages. Just as a function in C, JavaScript or Python will jump from the function call, to its definition, and then return back to the line of code following the call, subroutines will also allow us to execute instructions non-sequentially. 

The stack is used to hold the return address used by `RET`, so you **must** implement the
stack in step 11, first. Then, add subroutine instructions `CALL` and `RET`.

* For `CALL`, you will likely have to modify your handler call in `cpu_run()`. The
problem is that some instructions want to execute and move to the next
instruction like normal, but others, like `CALL` and `JMP` want to go to a
specific address.

  > Note: `CALL` is very similar to the `JMP` instruction. However, there is one key difference between them. Can you find it in the specs? 
  > 

  * In **any** case where the instruction handler sets the `PC` directly, you
_don't_ want to advance the PC to the next instruction. So you'll have to set up
a special case for those types of instructions. This can be a flag you
explicitly set per-instruction... but can also be computed from the value in
`IR`. Check out the spec for more.

If you run `./ls8 examples/call.ls8` you should see the output:

```
20
30
36
60
```

## Stretch Goal: Timer Interrupts

Add interrupts to the LS-8 emulator.

**You must have implemented a CPU stack before doing this.**

**You must have implmented the `ST` instruction before doing this.**

See the [LS-8
spec](https://github.com/LambdaSchool/Computer-Architecture-One/blob/master/LS8-SPEC.md)
for details on implementation.

The LS-8 should fire a timer interrupt one time per second. This could be
implemented by calling `gettimeofday()` each iteration of the main loop and
checking to see if one second has elapsed.

Another option is the `setitimer()` call, which requires you to set up a signal
handler for the timer when it expires.

When the timer is ready to fire, set bit 0 of the IS register (R6).

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

10000010 # LDI R0,0XF8
00000000
11111000
10000010 # LDI R1,INTHANDLER
00000001
00010001
10000100 # ST R0,R1
00000000
00000001
10000010 # LDI R5,1
00000101
00000001
10000010 # LDI R0,LOOP
00000000
00001111

# LOOP (address 15):
01010100 # JMP R0
00000000

# Timer interrupt Handler
# When the timer interrupt occurs, output an 'A'
# INTHANDLER (address 17):
10000010 # LDI R0,65
00000000
01000001
01001000 # PRA R0
00000000
00010011 # IRET
```


The assembly program is interested in getting timer interrupts, so it sets the
IM register to `00000001` with `LDI R5,1`.

The interrupt timer gets to 1 second, and sets bit #0 in IS.

At the beginning of each `cpu_run()` loop, the CPU checks to see if interrupts
are enabled. If not, it continues processing instructions as normal. Otherwise:

Bitwise-AND the IM register with the IS register. This masks out all the
interrupts we're not interested in, leaving the ones we are interested in:

```c
interrupts = cpu.reg[IM] & cpu.reg[IS];
```

Step through each bit of `interrupts` and see which interrupts are set.

```c
for (int i = 0; i < 8; i++) {
  // Right shift interrupts down by i, then mask with 1 to see if that bit was set
  int interrupt_happened = ((interrupts >> i) & 1) == 1;

  // ...
}
```

(If the no interrupt bits are set, then stop processing interrupts and continue
executing the current instruction as per usual.)

If `interrupt_happened`, check the LS-8 spec for details on what to do.


## Stretch Goal: Keyboard Interrupts

This gets into some serious Unix system programming esoterica. Google for
`tcsetattr` and `raw mode` and `non-blocking`.

If you find the command line is no longer showing what you type after your
program exits, you should type this command (in the blind) to return it to
normal:

```
stty sane
```