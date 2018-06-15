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
const PUSH = 0b01001101;
const POP = 0b01001100;
const CALL = 0b01001000;
const RET = 0b00001001;
const CMP = 0b10100000;
const JMP = 0b01010000;
const JEQ = 0b01010001;
const JNE = 0b01010010;

let SP = 7;

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
        this.FL = {
            0: 0,
            0: 0,
            0: 0,
            0: 0,
            0: 0,
            L: 0,
            G: 0,
            E: 0,
        };

        this.reg[SP] = 0xf4;
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
                this.reg[regA] = (this.reg[regB] * this.reg[regA]) & 0xff;
                break;
            case 'ADD':
                this.reg[regA] = (this.reg[regB] + this.reg[regA]) & 0xff;
                break;
            case 'SUB':
                this.reg[regA] = (this.reg[regA] - this.reg[regB]) & 0xff;
                break;
            case 'DIV':
                if(regB === 0) {
                    IR = HLT;
                    console.error('Cannot divide by 0');
                    break;
                }
                this.reg[regA] = (this.reg[regA] / this.reg[regB]) & 0xff;
                break;
            case 'INC':
                this.reg[regA] = this.reg[regA] + 1;
                break;
            case 'DEC':
                this.reg[regA] = this.reg[regA] - 1;
                break;
            case 'CMP':
                console.log("CMP A: ", regA, " B: ", regB);
                if(this.reg[regA] === this.reg[regB]) {
                    this.FL.E = 1;
                    this.FL.L = 0;
                    this.FL.G = 0;
                } else if (this.reg[regA] < this.reg[regB]) {
                    this.FL.E = 0;
                    this.FL.L = 1;
                    this.FL.G = 0;
                } else if (this.reg[regA] > this.reg[regB]) {
                    this.FL.E = 0;
                    this.FL.L = 0;
                    this.FL.G = 1;
                }
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

        const IR = this.ram.mem[this.PC];

        // Debugging output
        // console.log(`${this.PC}: ${IR.toString(2)}`);

        // Get the two bytes in memory _after_ the PC in case the instruction
        // needs them.

        const operandA = this.ram.read(this.PC+1);
        const operandB = this.ram.read(this.PC+2);

        // Execute the instruction. Perform the actions for the instruction as
        // outlined in the LS-8 spec.
        this.pcAdvance = true;

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
                this.alu('MUL', operandA, operandB);
                break;

            case ADD:
                this.alu('ADD', operandA, operandB);
                break;

            case SUB:
                this.alu('SUB', operandA, operandB);
                break;

            case DIV:
                this.alu('DIV', operandA, operandB);
                break;

            case INC:
                this.alu('INC', operandA);
                break;
            
            case DEC:
                this.alu('DEC', operandA);
                break;

            case PUSH:
                console.log("SP starting at: ", this.reg[SP]);
                this.reg[SP]--;
                this.ram.write(this.reg[SP], this.reg[operandA]);
                console.log("SP pushed to: ", this.reg[SP]);
                break;

            case POP:
                console.log("SP starting at: ", this.reg[SP]);
                this.reg[operandA] = this.ram.read(this.reg[SP]);
                this.reg[SP]++;
                console.log("SP popped to: ", this.reg[SP]);
                break;

            case CALL:
                console.log("SP starting at: ", this.reg[SP]);
                this.pushValue(this.PC + 2);
                this.PC = this.reg[operandA];
                this.pcAdvance = false;
                console.log("SP called to: ", this.reg[SP]);
                break;

            case RET:
                this.PC = this.reg[SP];
                break;

            case CMP:
                this.alu('CMP', operandA, operandB);
                break;

            case JMP:
                console.log("JMP");
                // this.ram.mem = operandA;
                this.PC = this.reg[operandA];
                this.pcAdvance = false;
                break;

            case JEQ:
                console.log("JEQ");
                console.log(this.FL);
                if(this.FL.E === 0b00000001) {
                    console.log("JEQ JUMP");
                    this.PC = this.reg[operandA];
                    this.pcAdvance = false;
                }
                break;

            case JNE:
                console.log("JNE");
                console.log(this.FL);
                if(this.FL.E === 0b00000000) {
                    console.log("JNE JUMP");
                    this.PC = this.reg[operandA];
                    this.pcAdvance = false;
                }
                break;

            default:
                console.log("Defaulted case, PC: " , this.PC);
                break;
        }

        // Increment the PC register to go to the next instruction. Instructions
        // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
        // instruction byte tells you how many bytes follow the instruction byte
        // for any particular instruction.
        if(this.pcAdvance) {
            const instLen = (IR >> 6) + 1;
            this.PC += instLen;
        }
    }

    pushValue(v) {
        this.reg[SP]--;
        this.ram.write(this.reg[SP], v);
    }
}

module.exports = CPU;
