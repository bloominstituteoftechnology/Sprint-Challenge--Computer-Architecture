/**
 * LS-8 v2.0 emulator skeleton code
 */

/**
 * Class for simulating a simple Computer (CPU & memory)
 */



const LDI = 0b10011001;
const PRN = 0b01000011;
const HLT = 0b00000001;
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
            case ADD:
                this.reg[regA] = this.reg[regA] + this.reg[regB];
                break;

            case PUSH:                       // initially --> this.reg[7] = 244, push R0 (2)
                this.reg[SP]--;                              // this.reg[7]-- 
                this.ram.write(this.reg[SP], this.reg[regA])// ram[243] = this.reg[R0] , (2)
                break;

            case POP:
                this.reg[regA] = this.ram.read(this.reg[SP])// this.reg[R0] <-- ram[243]  (2)
                this.reg[SP]++;                             // this.reg[SP]++               
                break;

            case CALL:
                this.reg[SP]--;
                this.ram.write(this.reg[SP], this.PC + 2)// next instruction
                // Set the PC to the address stored in register
                // this.pushValue(this.PC + 2)
                this.PC = this.reg[regA]
                break;

            case RET:
                this.PC = this.ram.read(this.reg[SP]);
                break;

            case MUL:
                const result = this.reg[regA] * this.reg[regB];
                this.reg[regA] = result;
                break;

            case LDI: // LDI
                this.reg[regA] = regB;// this.reg[R0] = 3
                break;

            case PRN: // PRN
                console.log(this.reg[regA])// this.reg[R0](2)
                break;

            case HLT: // HLT
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

        if (![CALL, RET].includes(IR)) {
            const instLen = (IR >> 6) + 1;
            this.PC += instLen;
        }
    }
}

module.exports = CPU;
