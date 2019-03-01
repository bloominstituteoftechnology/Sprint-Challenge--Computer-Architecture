# Computer Architecture

## Project

- [Implement the LS-8 Emulator](ls8/)

## Task List: add this to the first comment of your Pull Request

### Day 1: Get `print8.ls8` running

- [x] Inventory what is here
- [x] Implement `struct cpu` in `cpu.h`
- [x] Add RAM functions `cpu_ram_read` and `cpu_ram_write`
- [x] Implement `cpu_init()`
- [x] Implement the core of `cpu_run()`
- [x] Implement the `HLT` instruction handler
- [x] Add the `LDI` instruction
- [x] Add the `PRN` instruction

### Day 2: Add the ability to load files dynamically, get `mult.ls8` and `stack.ls8` running

- [x] Un-hardcode the machine code
- [x] Implement the `cpu_load` function to load an `.ls8` file given the filename passed in as an argument
- [x] Implement a Multiply instruction and Print the result (run `mult8.ls8`)
- [x] Implement the System Stack and be able to run the `stack.ls8` program

### Day 3: Get `call.ls8` running

- [x] Implement the CALL and RET instructions
- [x] Implement Subroutine Calls and be able to run the `call.ls8` program

### Stretch

- [ ] Add the timer interrupt to the LS-8 emulator
- [ ] Add the keyboard interrupt to the LS-8 emulator
