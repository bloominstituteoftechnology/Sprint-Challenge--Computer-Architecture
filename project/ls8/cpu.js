/**
 * LS-8 v2.0 emulator skeleton code
 */
const LDI = 0b10011001;
const PRN = 0b01000011;
const HLT = 0b00000001;
const MUL = 0b10101010;
const PUSH = 0b01001101;
const POP = 0b01001100;
const CALL = 0b01001000;
const MULT2PRINT = 0b00011000;
const CMP = 0b10100000;
const JMP = 0b01010000;
const JEQ = 0b01010001;
const JNE = 0b01010010;

/**
 * Class for simulating a simple Computer (CPU & memory)
 */
class CPU {

    /**
     * Initialize the CPU
     */
    constructor(ram) {
        this.ram = ram;

        this.reg = new Array(8).fill(0); // General-purpose registers R0-R7
        
        // Special-purpose registers
        this.PC = 0; // Program Counter
        this.FL = 0;
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
                return regA * regB;
            case 'CMP':
                this.FL = 0;
                if (regA === regB) {
                    this.FL |= 0b001;
                } else if (regA > regB) {
                    this.FL |= 0b010;
                } else if (regA < regB) {
                    this.FL |= 0b100;
                }
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
        const operandA = this.ram.read(this.PC+1);
        const operandB = this.ram.read(this.PC+2);

        // Execute the instruction. Perform the actions for the instruction as
        // outlined in the LS-8 spec.
        let advance = true;
        // !!! IMPLEMENT ME
        switch(IR) {
            case LDI:
                this.reg[operandA] = operandB;
                break;
            case PRN:
                console.log(this.reg[operandA]);
                break;
            case HLT:
                this.stopClock();
                break;
            case MUL:
                this.reg[operandA] = this.alu('MUL', this.reg[operandA], this.reg[operandB]);
                break;
            case PUSH:
                this.ram.SP--;
                this.ram.mem[this.ram.SP] = this.reg[operandA];
                break;
            case POP:
                this.reg[operandA] = this.ram.mem[this.ram.SP];
                this.ram.mem[this.ram.SP] = 0;
                this.ram.SP = Math.min(this.ram.SP+1, 0xF4);
                break;
            case CALL:
                this.ram.SP--;
                this.ram.mem[this.ram.SP] = this.PC+2;
                advance = false;
                this.PC = operandA;
            case MULT2PRINT:
                console.log(this.alu('MUL', this.reg[0], 2));
                this.PC = this.ram.mem[this.ram.SP];
                this.ram.mem[this.ram.SP] = 0;
                this.ram.SP = Math.min(this.ram.SP+1, 0xF4);
                advance = false;
                break;
            case CMP:
                this.alu('CMP', this.reg[operandA], this.reg[operandB]);
                break;
            case JMP:
                this.PC = this.reg[operandA];
                advance = false;
                break;
            case JEQ:
                if (this.FL === 0b001) {
                    this.PC = this.reg[operandA];
                    advance = false;
                } break;
            case JNE:
                if (this.FL !== 0b001) {
                    this.PC = this.reg[operandA];
                    advance = false;
                } break;
            default:
                console.log(`unknown value at ${this.PC}: ${IR.toString(2)}`)
                this.stopClock();
                break;
        }

        // Increment the PC register to go to the next instruction. Instructions
        // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
        // instruction byte tells you how many bytes follow the instruction byte
        // for any particular instruction.
        
        // !!! IMPLEMENT ME
        if (advance === true) {
            this.PC += (IR >> 6) + 1;
        }
    }
}

module.exports = CPU;
