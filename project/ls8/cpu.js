/**
 * LS-8 v2.0 emulator skeleton code
 */

const LDI = 0b10011001;
const PRN = 0b01000011;
const HLT = 0b00000001;

const MUL = 0b10101010;
const ADD = 0b10101000;
const SUB = 0b10101001;
const DIV = 0b10101011;
const INC = 0b01111000;
const DEC = 0b01111001;
const LD = 0b10011000;
const PRA = 0b01000010;
const AND = 0b10110011;

const POP = 0b01001100;
const PUSH = 0b01001101;
const CALL = 0b01001000;
const RET = 0b00001001;
const ST = 0b10011010;
const CMP = 0b10100000;
const JMP = 0b01010000;
const JEQ = 0b01010001;
const JNE = 0b01010010;

/**
 * Class for simulating a simple Computer (CPU & memory)
 */

const SP = 7;

class CPU {

    /**
     * Initialize the CPU
     */
    constructor(ram) {
        this.ram = ram;

        this.reg = new Array(8).fill(0); // General-purpose registers R0-R7
        
        this.reg[SP] = 0xf4; // Stack Pointer Starting empty at Address F4

        // Special-purpose registers
        this.PC = 0; // Program Counter

        this.FL = 0b00000000;
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
            case 'ADD':
                this.reg[regA] += this.reg[regB];
                break;

            case 'MUL':
                this.reg[regA] *= this.reg[regB];
                // !!! IMPLEMENT ME
                break;

            case 'SUB':
                this.reg[regA] -= this.reg[regB];

            case 'DIV':
                if (regB === 0) {
                    // console.error('Denominator cannot be zero.');
                    this.stopClock();
                } else {
                    this.reg[regA] /= this.reg[regB];
                }
                break;

            case 'AND':
                this.reg[regA] &= this.reg[regB];
                break;

            case 'INC':
                this.reg[regA]++;
                break;

            case 'DEC':
                this.reg[regA]--;
                break;

            default:
                console.log("Default");
                break;
        }
    }

    /**
     * Advances the CPU one cycle
     */
    tick() {
        const push_it = x => {
            this.alu('DEC', SP);
            this.ram.write(this.reg[SP], x);
        };
      
        const pop_it = () => {
            const popped = this.ram.read(this.reg[SP]);
            this.alu('INC', SP);
            return popped;
        };

        // Load the instruction register (IR--can just be a local variable here)
        // from the memory address pointed to by the PC. (I.e. the PC holds the
        // index into memory of the instruction that's about to be executed
        // right now.)

        const IR = this.ram.read(this.PC);

        // !!! IMPLEMENT ME

        // Debugging output

        // console.log(`${this.PC}: ${IR.toString(2)}`);

        // Get the two bytes in memory _after_ the PC in case the instruction
        // needs them.

        const operandA = this.ram.read(this.PC + 1);
        const operandB = this.ram.read(this.PC + 2);

        // !!! IMPLEMENT ME

        // Execute the instruction. Perform the actions for the instruction as
        // outlined in the LS-8 spec.

        switch(IR) {
            case ADD:
                this.alu('ADD', operandA, operandB);
                break;

            case MUL:
                this.alu('MUL', operandA, operandB);
                break;

            case SUB:
                this.alu('SUB', operandA, operandB);
                break;

            case DIV:
                this.alu('DIV', operandA, operandB);
                break;

            case AND:
                this.alu('AND', operandA, operandB);
                break;

            case INC:
                this.alu('INC', operandA);
                break;

            case DEC:
                this.alu('DEC', operandA);
                break;

            case PRA:
                console.log(String.fromCharCode(this.reg[operandA]));
                break;

            case LD:
                this.reg[operandA] = this.reg[operandB];
                break;

            case JMP:
                // this isn't going to work since we increment PC after the switch statement
                this.PC = this.reg[operandA];
                break;

            case POP:
                this.reg[operandA] = pop_it();
                break;

            case PUSH:
                push_it(this.reg[operandA]);
                break;

            case CALL:
                this.alu('DEC', SP);
                push_it(this.PC + 2);
                this.PC = this.reg[operandA];
                break;

            case RET:
                this.PC = this.ram.read(this.reg[SP]);
                this.alu('INC', SP);
                break;

            case ST:
                this.ram.write(this.reg[operandA], this.reg[operandB]);
                break;

            case CMP:
                if (this.reg[operandA] < this.reg[operandB]) this.FL = 0b00000100;
                if (this.reg[operandA] > this.reg[operandB]) this.FL = 0b00000010;
                if (this.reg[operandA] === this.reg[operandB]) this.FL = 0b00000001;
                // else this.FL = 0b00000000;
                break;

            case JMP:
                this.PC = this.reg[operandA];
                break;

            case JEQ:
                if (this.FL === 0b00000001) {
                  this.PC = this.reg[operandA];
                } else {
                  this.PC += 1 + (IR >> 6);
                }
                break;

            case JNE:
                if (this.FL !== 0b00000001) {
                  this.PC = this.reg[operandA];
                } else {
                  this.PC += 1 + (IR >> 6);
                }
                break;

            case LDI:
                // Set the value in a register
                this.reg[operandA] = operandB;
                // this.PC += 3; // Next Instruction
                // console.log(this.regs[operandA]);
                break;

            case PRN:
                console.log(this.reg[operandA]);
                // this.PC += 2;
                break;

            case HLT:
                this.stopClock();
                // this.PC += 1;
                break;

            default:
                // console.log("Unknown Instruction: " + IR.toString(2));
                this.stopClock();
                return;
        }
        // !!! IMPLEMENT ME

        // Increment the PC register to go to the next instruction. Instructions
        // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
        // instruction byte tells you how many bytes follow the instruction byte
        // for any particular instruction.

        if (IR !== CALL && IR !== JMP && IR !== RET && IR !== JEQ && IR !== JNE) {
            this.PC += 1 + (IR >> 6);
        }
        // const instLen = (IR >> 6) + 1;
        // this.PC += instLen;

        // console.log(InstLen);
        // !!! IMPLEMENT ME
    }
}

module.exports = CPU;
