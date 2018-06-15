/**
 * LS-8 v2.0 emulator skeleton code
 */

/**
 * Class for simulating a simple Computer (CPU & memory)
 */

/*
`LDI register immediate`
Set the value of a register to an integer.
Machine code:
10011001 00000rrr iiiiiiii
*/
const LDI = 0b10011001;


/*
`PRN register` pseudo-instruction
Print numeric value stored in the given register.
Print to the console the decimal integer value that is stored in the given
register.
Machine code:
01000011 00000rrr
*/
const PRN = 0b01000011;


/*
`HLT`
Halt the CPU (and exit the emulator).
Machine code:
00000001 
*/
const HLT = 0b00000001;


/*
`MUL registerA registerB`
Multiply two registers together and store the result in registerA.
Machine code:
10101010 00000aaa 00000bbb
*/
const MUL = 0b10101010;
/* 
PUSH register
Push the given register on the stack.
Decrement the SP.
Copy the value in the given register to the address pointed to by SP.
Machine code:
01001101 00000rrr
*/
const PUSH = 0b01001101;

/*
POP register
Pop the value at the top of the stack into the given register.
Copy the value from the address pointed to by SP to the given register.
Increment SP.
Machine code:
01001100 00000rrr
 */
const POP = 0b01001100;

/*
CALL register
Calls a subroutine (function) at the address stored in the register.
The address of the next instruction that will execute is pushed onto the stack.
The PC is set to the address stored in the given register.
Machine code:
01001000 00000rrr
*/
const CALL = 0b01001000;

/*
RET
Return from subroutine.
Pop the value from the top of the stack and store it in the PC.
Machine Code:
00001001
*/
const RET = 0b00001001;



/*
### ADD
`ADD registerA registerB`
Add two registers and store the result in registerA.
Machine code:
```
10101000 00000aaa 00000bbb
*/
const ADD = 0b10101000;

//---------------------------sprint--------------------------------------------
//*****sprint

/**
`JMP register`
Jump to the address stored in the given register.
Set the `PC` to the address stored in the given register.
Machine code:
01010000 00000rrr
 */
const JMP = 0b01010000;


/*
`JEQ register`
If `equal` flag is set (true), jump to the address stored in the given register.
Machine code:
01010001 00000rrr
*/
const JEQ = 0b01010001;



/*
`JNE register`
If `E` flag is clear (false, 0), jump to the address stored in the given
register.
Machine code:
01010010 00000rrr
 */
const JNE = 0b01010010;


/*
`CMP registerA registerB`
Compare the value in two registers.
* If they are equal, set the Equal `E` flag to 1, otherwise set it to 0.
* If registerA is less than registerB, set the Less-than `L` flag to 1,
  otherwise set it to 0.
* If registerA is greater than registerB, set the Greater-than `G` flag
  to 1, otherwise set it to 0.
Machine code:
10100000 00000aaa 00000bbb

## Flags

The flags register `FL` holds the current flags status. These flags
can change based on the operands given to the `CMP` opcode.

The register is made up of 8 bits. If a particular bit is set, that flag is "true".

`FL` bits: `00000LGE`

* `L` Less-than: during a `CMP`, set to 1 if registerA is less than registerB,
zero otherwise.
* `G` Greater-than: during a `CMP`, set to 1 if registerA is greater than
registerB, zero otherwise.
* `E` Equal: during a `CMP`, set to 1 if registerA is equal to registerB, zero
otherwise.
*/
const CMP = 0b10100000;

//------------------------------------sprint----------------------------------


// R7 is reserved as the stack pointer (SP)
/*
      top of RAM
+-----------------------+
| FF  I7 vector         |    Interrupt vector table
| FE  I6 vector         |
| FD  I5 vector         |
| FC  I4 vector         |
| FB  I3 vector         |
| FA  I2 vector         |
| F9  I1 vector         |
| F8  I0 vector         |
| F7  Reserved          |
| F6  Reserved          |
| F5  Reserved          |
| F4  Key pressed  **SP?? |    Holds the most recent key pressed on the keyboard
| F3  Start of Stack    |
| F2  [more stack]      |    Stack grows down
| ...                   |
| 01  [more program]    |
| 00  Program entry     |    Program loaded upward in memory starting at 0
+-----------------------+
    bottom of RAM
*/

const SP = 7;
//CPU -------------------------------
class CPU {

    /**
     * Initialize the CPU
     */
    constructor(ram) {
        this.ram = ram;
        this.reg = new Array(8).fill(0); // General-purpose registers R0-R7
        // Special-purpose registers
        this.PC = 0; // Program Counter
        this.reg[SP] = 0xf4; // initialize the SP (F4 --> 244)

        this.FL = 0;//*****sprint
    }
    /**
     * Store value in memory address, useful for program loading
     */
    poke(address, value) {
        this.ram.write(address, value);
    }
    /**
     * Starts the clock ticking on the CPU
     */
    startClock() {
        this.clock = setInterval(() => {
            this.tick();
        }, 1); // 1 ms delay == 1 KHz clock == 0.000001 GHz
    }
    /**
     * Stops the clock
     */
    stopClock() {
        clearInterval(this.clock);
    }
    /**
     * ALU functionality
     *
     * The ALU is responsible for math and comparisons.
     *
     * If you have an instruction that does math, i.e. MUL, the CPU would hand
     * it off to it's internal ALU component to do the actual work.
     *
     * op can be: ADD SUB MUL DIV INC DEC CMP
     */
    alu(op, regA, regB) {

        // node ls8.js stack.ls8
        switch (op) {
            //---------------sprint--------------------------
            case JMP:
                // Set the `PC` to the address stored in the given register.
                this.PC = this.ram.read(this.reg[regA]) // (this.PC + 1) will it work? ******
                break;

            case JNE:
                // If `E` flag is clear (FL !== 0b00000001), jump to the address stored in the given register.
                if (this.FL !== 0b00000001) {
                    return (this.PC = this.reg[regA]);
                }
                break;


            case JEQ:
                // If `equal` flag is set (true), jump to the address stored in the given register.
                if (this.FL === 0b00000001) {
                    return (this.PC = this.reg[regA]);
                }
                break;

            case CMP:
                // `L` Less-than: during a `CMP`, set to 1 if registerA is less than registerB, zero otherwise. `00000LGE` --> `00000100`
                if (this.reg[regA] < this.reg[regB]) {
                    this.FL = 0b00000100;
                    // `G` Greater-than: during a `CMP`, set to 1 if registerA is greater than registerB, zero otherwise. `00000LGE` --> `00000010` 
                } else if (this.reg[regA] > this.reg[regB]) {
                    this.FL = 0b00000010;
                    // If they are equal, set the Equal `E` flag to 1, otherwise set it to 0. `00000LGE` --> `00000001` 
                } else if (this.reg[regA] === this.reg[regB]) {
                    this.FL = 0b00000001;
                }
                break;

            //---------------sprint--------------------------
            case ADD:
                this.reg[regA] = this.reg[regA] + this.reg[regB];
                break;

            case PUSH:                                       // initially --> this.reg[7] = 244, push R0 (2)
                this.reg[SP]--;                              // this.reg[7]-- 
                this.ram.write(this.reg[SP], this.reg[regA])// ram[243] = this.reg[R0] , (2)
                break;

            case POP:
                this.reg[regA] = this.ram.read(this.reg[SP])// this.reg[R0] <-- ram[243]  (2)
                this.reg[SP]++;                             // this.reg[SP]++               
                break;

            case CALL:
                // Calls a subroutine (function) at the address stored in the register.
                this.reg[SP]--;
                this.ram.write(this.reg[SP], this.PC + 2)// The address of the next instruction that will execute is pushed onto the stack.
                this.PC = this.reg[regA]; // The PC is set to the address stored in the given register.
                break;

            case RET:
                // Pop the value from the top of the stack and store it in the PC.
                this.PC = this.ram.read(this.reg[SP]);
                break;

            case MUL:
                // Multiply two registers together and store the result in registerA.
                const result = this.reg[regA] * this.reg[regB];
                this.reg[regA] = result;
                break;

            case LDI:
                // Set the value of a register to an integer.
                this.reg[regA] = regB;// this.reg[R0] = 3
                break;

            case PRN:
                // Print numeric value stored in the given register.
                console.log(this.reg[regA])
                break;

            case HLT:
                // Halt the CPU (and exit the emulator).
                this.stopClock()
                break;

            default:
                console.log(`Unrecognized instruction ${op.toString(2)}`)
        }
    }

    /**
     * Advances the CPU one cycle
     */
    tick() {
        // Load the instruction register (IR--can just be a local variable here)
        // from the memory address pointed to by the PC. (I.e. the PC holds the
        // index into memory of the instruction that's about to be executed
        // right now.)

        // !!! IMPLEMENT ME

        const IR = this.ram.read(this.PC);

        // Debugging output
        // console.log(`${this.PC}: ${IR.toString(2)}`);

        // Get the two bytes in memory _after_ the PC in case the instruction
        // needs them.

        // !!! IMPLEMENT ME
        const operandA = this.ram.read(this.PC + 1); // register number
        const operandB = this.ram.read(this.PC + 2);  // the value eg. 8 
        // console.log(afterPc1, afterPc2)

        // Execute the instruction. Perform the actions for the instruction as
        // outlined in the LS-8 spec.

        // !!! IMPLEMENT ME
        this.alu(IR, operandA, operandB);

        // const instructionCalls = instruction[IR]
        // this[instructionCalls](operandA, operandB)

        // Increment the PC register to go to the next instruction. Instructions
        // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
        // instruction byte tells you how many bytes follow the instruction byte
        // for any particular instruction.
        // !!! IMPLEMENT ME

        if (![CALL, RET, JMP].includes(IR)) {
            const instLen = (IR >> 6) + 1;
            this.PC += instLen;
        }
    }
}

module.exports = CPU;
