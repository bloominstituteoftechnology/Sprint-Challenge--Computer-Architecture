# Computer Architecture 

## Project

* [Implement the LS-8 Emulator](ls8/)

## Task List: add this to the first comment of your Pull Request

### Day 1: Get `print8.ls8` running
- [ ] Inventory what is here
- [ ] Implement `struct cpu` in `cpu.h`
- [ ] Add RAM functions `cpu_ram_read` and `cpu_ram_write`
- [ ] Implement `cpu_init()`
- [ ] Implement the core of `cpu_run()`
- [ ] Implement the `HLT` instruction handler
- [ ] Add the `LDI` instruction
- [ ] Add the `PRN` instruction

### Day 2: Add the ability to load files dynamically, get `mult.ls8` and `stack.ls8` running
- [ ] Un-hardcode the machine code
- [ ] Implement the `cpu_load` function to load an `.ls8` file given the filename passed in as an argument
- [ ] Implement a Multiply instruction and Print the result (run `mult8.ls8`)
- [ ] Implement the System Stack and be able to run the `stack.ls8` program

### Day 3: Get `call.ls8` running
- [ ] Implement the CALL and RET instructions
- [ ] Implement Subroutine Calls and be able to run the `call.ls8` program

### Stretch
- [ ] Add the timer interrupt to the LS-8 emulator
- [ ] Add the keyboard interrupt to the LS-8 emulator
