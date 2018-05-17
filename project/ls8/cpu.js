/**
 * LS-8 v2.0 emulator skeconston code
 */
const LDI = 0b10011001;
const PRN = 0b01000011;
const HLT = 0b00000001;
const MUL = 0b10101010;
const POP = 0b01001100;
const PUSH = 0b01001101;
const CALL = 0b01001000;
const RET  = 0b00001001;
const ADD  = 0b10101000;
const JMP  = 0b01010000;
const CMP  = 0b10100000;
const JEQ  = 0b01010001;
const JNE  = 0b01010010;

const L = 0b100,
      E = 0b001,
      G = 0b010;

const SP = 7 // Stack
/**
 * Class for simulating a simple Computer (CPU & memory)
 */
class CPU {

    /**
     * Initialize the CPU
     */
    constructor(ram) {
        this.ram = ram;
        this.reg[SP] = 0xf4;

        this.reg = new Array(8).fill(0); // General-purpose registers R0-R7
        
        // Special-purpose registers
        this.reg.PC = 0; // Program Counter
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
    stoPClock() {
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
                this.reg[regA] *= this.reg[regB];
                break;
            case 'ADD':
                this.reg[regA] += this.reg[regB];
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
        const IR = this.ram.read(this.reg.PC);
        // !!! IMPLEMENT ME

        // Debugging output
        //console.log(`${this.reg.PC}: ${IR.toString(2)}`);

        // Get the two bytes in memory _after_ the PC in case the instruction
        // needs them.
        const operandA = this.ram.read(this.reg.PC + 1);
        const operandB = this.ram.read(this.reg.PC + 2);
        // !!! IMPLEMENT ME

        // Execute the instruction. Perform the actions for the instruction as
        // outlined in the LS-8 spec.

        switch(IR) {
            case LDI:
                this.reg[operandA] = operandB;
                break;
            case PRN:
                console.log(this.reg[operandA]);
                break;
            case HLT: 
                this.stoPClock();
                break;
            case MUL: 
                this.alu('MUL', operandA, operandB);
                break;
            case POP:
                this.reg[operandA] = this.ram.read(this.reg[SP]);
                this.reg[SP] += 1;
                break;
            case PUSH:
                this.reg[SP] -= 1;
                this.ram.write(this.reg[SP], this.reg[operandA]);
                break;
            case CALL: 
                this.reg[SP] -= 1;
                this.ram.write(this.reg[SP], this.reg.PC + 1);
            case RET: 
                this.reg.PC = this.ram.read(this.reg[SP]);
                this.reg[SP] += 1;
                break;
            case ADD:
                this.alu('ADD', operandA, operandB);
                break;
            case JMP: 
                this.flag = true;
                this.reg.PC = this.reg[operandA];
                break;
            case CMP:
                this.alu('CMP', operandA, operandB);
                break;
            case JEQ:
                if (this.flag === 1) {
                    this.flag = true;
                    this.reg.PC = this.reg[operandA];
                }
                break;
            case JNE:
                if (this.flag !== 1) {
                    this.reg.PC = this.reg[operandA];
                    this.flag = true;
                }
                break;
        }

        // !!! IMPLEMENT ME

        // Increment the PC register to go to the next instruction. Instructions
        // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
        // instruction byte tells you how many bytes follow the instruction byte
        // for any particular instruction.

        // 0b10011001 >> 6 == 0b00000010;

        // 0000000010 & 00000011 == 00000010;

        if(!this.flag) {

        const operandCount = (IR >>> 6) & 0b11;
        const totalInstructionLen = operandCount + 1;
        this.reg.PC += totalInstructionLen;
    }
        // !!! IMPLEMENT ME
        this.flag = false;
    }
}

module.exports = CPU;
