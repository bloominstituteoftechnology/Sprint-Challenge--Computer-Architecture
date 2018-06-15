/**
 * LS-8 v2.0 emulator skeleton code
 */
const LDI = 0b10011001;
const PRN = 0b01000011;
const HLT = 0b00000001;
const MUL = 0b10101010;
const PUSH = 0b01001101;
const POP = 0b01001100;

const SP = 7;
//init stack pointer

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

        this.reg[SP] = 0xF4;
        
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
                this.reg[regA] = (this.reg[regB] * this.reg[regA]) & 0xff; 
                // prevents above 255.
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

        // !!! IMPLEMENT ME
        const IR = this.ram.read(this.PC);
        // we need to read the memory address thats
        //stored in register PC, and set it to IR.

        // Debugging output
        //console.log(`${this.PC}: ${IR.toString(2)}`);

        // Get the two bytes in memory _after_ the PC in case the instruction
        // needs them.

        // !!! IMPLEMENT ME

        const operandA = this.ram.read(this.PC + 1); // second byte
        const operandB = this.ram.read(this.PC + 2); // third byte

        // Execute the instruction. Perform the actions for the instruction as
        // outlined in the LS-8 spec.

        // !!! IMPLEMENT ME
        //this will be the switch statement for our instructions.
        //define them outside the class.
        switch(IR) { // IR holds a copy of the currently executing instruction
            case LDI:
                //set the value in a register
                this.reg[operandA] = operandB; 
                // set 8(regb) into reg0(opA)
                // LDI, REG0, 8: this.reg at regA is set to 8. 
                //operand A is the reg, so operandB must be the value
                //PC needs to move 3 ahead now that we used 3 bytes
                //this.PC += 3; 
                // sets the PC 3 bytes ahead to stop the infinite loop
                break;

            case PRN:
                //print a value in a register.
                console.log(this.reg[operandA]); 
                //opA now set to opB. basically printing opB
                //this.PC += 2;
                // sets the PC 2 bytes ahead.
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
                this.ram.write(this.reg[SP], this.reg[operandA]);
                break;

            case POP:
            this.reg[operandA] = this.ram.read(this.reg[SP]);
            this.reg[SP]++;
                break;

            default:
                console.log('Unknown instruction: ' + IR.toString());
                this.stopClock();
                return;
        }

        // Increment the PC register to go to the next instruction. Instructions
        // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
        // instruction byte tells you how many bytes follow the instruction byte
        // for any particular instruction.
        // !!! IMPLEMENT ME
        const instLen = (IR >> 6) + 1;
        this.PC += instLen;
    }
}

module.exports = CPU;

