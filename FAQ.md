# Computer Architecture FAQ
<!-- ============================================================================= -->

<p><details><summary><b>Once we get the <tt>HLT</tt> instruction, what should the emulator do?</b></summary><p>

You should exit the emulator.

If you `malloc()`d any memory, be sure to `free()` it.

You don't need to worry about any of the LS-8 internals at that point since
you're exiting anyway.

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>How do you return values from subroutines?</b></summary><p>

Since the `RET` instruction doesn't allow you to specify a return value, you'll
have to get the value back by other means.

One of the most common is to set a register (e.g. `R0`) to the return value, and
the caller will just know, by convention, that the `R0` register will hold that
value once the `CALL` returns.

But you could also push that value on the stack and have the caller pop it off.
This would have the advantage of supporting an arbitrary number of return
values.

There are no fixed rules when writing code in assembly language. Returning
values in registers just happens to be a common convention.

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>With interrupts, why do we push everything on the stack?</b></summary><p>

The idea is that if you save the machine state on the stack, then after you
service the interrupt you can restore it and seamlessly pick up where you left
off.

The CPU might have been in the middle of something important when the interrupt
occurred, and it'll want to get back to that once the interrupt handler is
complete.

So we push the general purpose registers and internal registers on the stack,
then do interrupt stuff, then restore all those registers from the stack so the
CPU can carry on with what it was doing before the interrupt occurred.

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>What is "stack overflow"?</b></summary><p>

Short answer: it's when the stack grows into some area of memory that something
else was using.

In the LS-8, this would mean the stack grew down in RAM to the point that it
overwrote some of the instructions in the program.

With a C program, this would mean the stack grew down and impacted the heap. (Or
that the heap grew up and impacted the stack.)

On modern machines with [virtual
memory](https://en.wikipedia.org/wiki/Virtual_memory), this isn't a practical
concern since you'll run out of physical RAM before the stack overflow occurs.

Some interpreted languages like Python track how large their internal stacks
have grown and crash out if the stack grows too large. But this is happening
within the Python virtual machine, not on the hardware.

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>On the LS-8, why does the stack pointer start at address <tt>F4</tt>, when the first stack element is at <tt>F3</tt>?</b></summary><p>

Since the first thing a `PUSH` instruction does is decrement the stack pointer,
it means that the stack pointer is moved to `F3` first and _then_ the value is
stored there. Exactly where we wanted it.

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>How are stacks and subroutines used by higher-level languages like C?</b></summary><p>

In C, when you make a function call, a bunch of space is allocated (pushed) on
the stack to hold a number of things:

* The return address to come back to after the function completes
* Space for all the function parameters
* Space for all the other local variables in the function

This allocated chunk of stack is called a [stack
frame](https://en.wikipedia.org/wiki/Call_stack#STACK-FRAME).

When you call any function (including when `main()` gets called in C):

1. A new stack frame is allocated (pushed)
2. Parameter values are copied from the function arguments to their spots on the
   stack frame

When you return from any function:

1. Any return value is copied from the stack frame into a dedicated register
2. The stack frame is deallocated (popped)

In assembly language, `CALL` doesn't allow any arguments to be passed, and `RET`
doesn't allow any values to be returned.

Using stack frames gives `CALL` the power to give parameters to subtroutines.

And we can use a dedicated register, like `R0`, to pass returned values back to
the caller over a `RET` instruction.

Since all the local variables for a function are stored in the stack frame, they
all vaporize as soon as the stack is popped when the function returned. This is
why local variables are not persistent from call to call.

Furthermore, using the stack to hold frames allows us to call functions to an
arbitrary nesting level. Indeed, it is what allows for recursion at all.

</p></details></p>

<!-- ============================================================================= -->

<p><details><summary><b>Is the flags <tt>FL</tt> register one of the <tt>Rx</tt> registers, or is it a special register?</b></summary><p>

It's a special purpose register that can be added separately to the `struct cpu`
similar to how `PC` works.

</p></details></p>


<!--

TODO:
-->

<!-- ============================================================================= -->

<!--
Template:

<p><details><summary><b></b></summary><p>
</p></details></p>
-->
