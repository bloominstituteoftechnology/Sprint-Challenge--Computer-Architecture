/**
 * LS-8 v2.0 emulator skeleton code
 */

const Op = {
    LDI: 0b10011001,
    PRN: 0b01000011,
    HLT: 0b00000001,
    MUL: 0b10101010,
    PUSH: 0b01001101,
    POP: 0b01001100,
    CALL: 0b01001000,
    RET: 0b00001001,
    ADD: 0b10101000
}

const SP = 7;

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

        this.reg[SP] = 244;
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
                return this.reg[regA] * this.reg[regB];
            case 'ADD':
                return this.reg[regA] + this.reg[regB];
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
        const IR = this.ram.read(this.PC);

        // Debugging output
        //console.log(`${this.PC}: ${IR.toString(2)}`);

        // Get the two bytes in memory _after_ the PC in case the instruction
        // needs them.
        const IR2 = this.ram.read(this.PC + 1);
        const IR3 = this.ram.read(this.PC + 2);

        // Execute the instruction. Perform the actions for the instruction as
        // outlined in the LS-8 spec.
        switch(IR) {
            case Op.LDI:
                this.reg[IR2] = IR3;
                this.PC += 3;
                break;

            case Op.PRN:
                console.log(this.reg[IR2]);
                this.PC += 2;
                break;

            case Op.MUL:
                this.reg[IR2] = this.alu('MUL', IR2, IR3);
                this.PC += 3;
                break;

            case Op.ADD:
                this.reg[IR2] = this.alu('ADD', IR2, IR3);
                this.PC += 3;
                break;

            case Op.PUSH:
                this.reg[SP]--;
                this.ram.write(this.reg[SP], this.reg[IR2]);
                this.PC += 2;
                break;

            case Op.POP:
                this.reg[IR2] = this.ram.read(this.reg[SP]);
                this.reg[SP]++;
                this.PC += 2;
                break;

            case Op.CALL:
                this.reg[SP]--;
                this.ram.write(this.reg[SP], this.PC + 2);
                this.PC = this.reg[IR2];
                break;

            case Op.RET:
                this.PC = this.ram.read(this.reg[SP]);
                this.reg[SP]++;
                break;

            default:
                console.log('Unhandled Op Code: ', IR.toString(2));
            case Op.HLT:
                this.stopClock();
                break;
        }

        // Increment the PC register to go to the next instruction. Instructions
        // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
        // instruction byte tells you how many bytes follow the instruction byte
        // for any particular instruction.
        
        // !!! IMPLEMENT ME
    }
}

module.exports = CPU;
