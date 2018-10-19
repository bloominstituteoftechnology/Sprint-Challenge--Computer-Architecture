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
const CMP = 0b10100000;
const JMP = 0b01010000;
const JEQ = 0b01010001;
const JNE = 0b01010010;

const SP = 7

this.FL = 0; //flag for CMP
this.JP = 0; //flag for JMP


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

        this.reg[SP] = 0xF4

        // Special-purpose registers
        this.PC = 0; // Program Counter
    }

    /**
     * Store value in memory address, useful for program loading
     */
    poke(address, value) {
        this
            .ram
            .write(address, value);
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
        const MUL = '10101010';
        switch (op) {
            case 'MUL': //(multiply)
                // !!! IMPLEMENT ME should multiply stuff
                this.reg[regA] *= this.reg[regB]
                break;
                // case 'ADD': regA = regA + regB return; break;
        }
    }

    /**
     * Advances the CPU one cycle
     */
    tick() {
        // Load the instruction register (IR--can just be a local variable here) from
        // the memory address pointed to by the PC. (I.e. the PC holds the index into
        // memory of the instruction that's about to be executed right now.) !!!
        // IMPLEMENT ME
        const IR = this
            .ram
            .read(this.PC)

        // Debugging output console.log(`${this.PC}: ${IR.toString(2)}`); Get the two
        // bytes in memory _after_ the PC in case the instruction needs them. !!!
        // IMPLEMENT ME
        const operandA = this
            .ram
            .read(this.PC + 1);
        const operandB = this
            .ram
            .read(this.PC + 2)

        // Execute the instruction. Perform the actions for the instruction as outlined
        // in the LS-8 spec. !!! IMPLEMENT ME
        switch (IR) {
            case LDI:
                //sets value in register
                this.reg[operandA] = operandB;
                break;
            case PRN:
                //debugging
                console.log(this.reg[operandA]);
                break;
            case HLT:
                this.stopClock();
                //this.PC += 1;
                break;
            case MUL:
                this.alu('MUL', operandA, operandB);
                break;
            case PUSH:
                this.reg[SP]--;
                this
                    .ram
                    .write(this.reg[SP], this.reg[operandA]);
                break;
            case POP:
                this.reg[operandA] = this
                    .ram
                    .read(this.reg[SP]);
                this.reg[SP]++;
                break;
            // case CALL: for calls and stuff still WIP
            //     this.reg
            //     break;
            case CMP:
                this.reg[operandA] === this.reg[operandB]
                    ? (this.FL = 0b000 | 0b001)
                    : this.reg[operandA] > this.reg[operandB]
                        ? (this.FL = 0b000 | 0b010)
                        : (this.FL = 0b000 | 0b100);
                break;
            case JMP: //jumps to address in given register
                this.PC = this.reg[operandA];
                this.JMP = 1;
            default:
                console.log('Unkown instruction: ' + IR.toString(2)); //tells you if something is invalid when you pop that in
                this.stopClock();
                return;
        }

        // Increment the PC register to go to the next instruction. Instructions can be
        // 1, 2, or 3 bytes long. Hint: the high 2 bits of the instruction byte tells
        // you how many bytes follow the instruction byte for any particular
        // instruction. !!! IMPLEMENT ME
        const instLen = (IR >> 6) + 1;
        this.PC += instLen;
    }
}

module.exports = CPU;
