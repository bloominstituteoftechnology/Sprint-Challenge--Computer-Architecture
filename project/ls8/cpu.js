/**
 * LS-8 v2.0 emulator skeleton code
 */

/**
 * Class for simulating a simple Computer (CPU & memory)
 */


const NOP = 0b00000000;
const LDI = 0b10011001;
const PRN = 0b01000011;
const MUL = 0b10101010;
const HLT = 0b00000001;
const JMP = 0b01010000;
const CALL = 0b01001000;
const RET = 0b00001001;
const PUSH = 0b01001101;
const POP = 0b01001100;

const SP = 7; //jeez 
class CPU {

    /**
     * Initialize the CPU
     */
    constructor(ram) {
        this.ram = ram;

        this.reg = new Array(8).fill(0); // General-purpose registers R0-R7
        this.reg[SP] = 0xf4; //didnt know you could just reference it like this lol

        // Special-purpose registers
        this.PC = 0; // Program Counter
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
        switch (op) {
            case 'MUL':
                // !!! IMPLEMENT ME
                break;
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
        let IR = this.ram.read(this.PC);
        // !!! IMPLEMENT ME

        // Debugging output
        // console.log(`${this.PC}: ${IR.toString(2)}`);

        // Get the two bytes in memory _after_ the PC in case the instruction
        // needs them.

        // !!! IMPLEMENT ME
        let operandA = this.ram.read(this.PC+1);
        let operandB = this.ram.read(this.PC+2);

        // Execute the instruction. Perform the actions for the instruction as
        // outlined in the LS-8 spec.
        // const ADD = (operandA, operandB) => {
        //     return operandA += operandB;
        // }

        // const AND = (operandA, operandB) => {
        //     if (operandA === operandB) {
        //         return operandA = true;
        //     } else {
        //         return operandA = false;
        //     }
        // } 
        // console.log(`inititalSP: ${SP}`)
        switch(IR) {
            case LDI:
                this.reg[operandA] = operandB;
                // this.PC += 3;
                break;

            case PRN:
                console.log(this.reg[operandA]);
                // this.PC += 2;
                break;

            case HLT:
                this.stopClock();
                break;
            
            case MUL:
                 this.reg[operandA] *= this.reg[operandB];
                break;

            case PUSH:
                this.reg[SP]--;
                this.ram.write(this.reg[SP], this.reg[operandA])
                // console.log(this.reg[operandA])
            case POP:
                this.reg[operandA] = this.ram.read(this.reg[SP]);
                this.reg[SP]++;
                // console.log(this.reg[operandA])

            case NOP:
                break;

            default:
            console.log(`Unknown Instruction At ${this.PC}: ${IR.toString(2)}`);
            this.stopClock();
        }

        // !!! IMPLEMENT ME

        // Increment the PC register to go to the next instruction. Instructions
        // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
        // instruction byte tells you how many bytes follow the instruction byte
        // for any particular instruction.
        
        // !!! IMPLEMENT ME
        if(IR !== CALL && IR !== JMP && IR !== RET) {
            this.PC += (IR >> 6) + 1;
        }
    }
}


module.exports = CPU;
