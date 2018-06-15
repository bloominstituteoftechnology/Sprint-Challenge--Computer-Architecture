/**
 * LS-8 v2.0 emulator skeleton code
 */

/**
 * Class for simulating a simple Computer (CPU & memory)
 */

 const SP = 7; // Stack Pointer
class CPU {

    /**
     * Initialize the CPU
     */
    constructor(ram) {
        this.ram = ram;

        this.reg = new Array(8).fill(0); // General-purpose registers R0-R7

        this.reg[SP] = 0xF4; // Init stack point (register 7 = 244 in hex);
        
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
                this.reg[regA] = this.reg[regA] * this.reg[regB];
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

        // Debugging output
        // console.log(`${this.PC}: ${IR.toString(2)}`);

        // Get the two bytes in memory _after_ the PC in case the instruction
        // needs them.
        const operandA = this.ram.read(this.PC + 1);
        const operandB = this.ram.read(this.PC + 2);

        // Execute the instruction. Perform the actions for the instruction as
        // outlined in the LS-8 spec.
        switch (IR) {
            // LDI
            case 0b10011001:
                // set the value in a register
                this.reg[operandA] = operandB;
                this.PC += 3;
                break;
            
            // PRN
            case 0b1000011:
                console.log(this.reg[operandA]);
                this.PC += 2;
                break;

            // MUL
            case 0b10101010:
                this.alu('MUL', operandA, operandB);
                this.PC += 3;
                break;

            // HLT
            case 0b00000001:
                this.stopClock();
                this.PC += 1;
                break;

            // PUSH STACK
            case 0b01001101:
                this.reg[SP]--;
                this.ram.write(this.reg[SP], this.reg[operandA]);
                this.PC += 2;
                break;

            // POP STACK
            case 0b01001100:
                this.reg[operandA] = this.ram.read(this.reg[SP]);
                this.reg[SP]++;
                this.PC += 2;
                break;

            default:
                console.log('Unknown instruction: ' + IR.toString(2));
                this.stopClock();
                return;
        }

        // Increment the PC register to go to the next instruction. Instructions
        // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
        // instruction byte tells you how many bytes follow the instruction byte
        // for any particular instruction.
        
        // !!! IMPLEMENT ME
    }
}

module.exports = CPU;
