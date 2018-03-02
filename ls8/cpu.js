/**
 * LS-8 v2.0 emulator skeleton code
 */

const fs = require('fs');

// Instructions

const HLT = 0b00000001; // Halt CPU
const ADD = 0b10101000;
const LDI = 0b10011001;
const MUL = 0b10101010;
const PRN = 0b01000011;
const CMP = 0b10100000; // CMP R R
const JMP = 0b01010000; // JMP R
const JEQ = 0b01010001; // JEQ R
const JNE = 0b01010010; // JNE R

/**
 * Class for simulating a simple Computer (CPU & memory)
 */
class CPU {

    /**
     * Initialize the CPU
     */
    constructor(ram) {
        this.ram = ram;

        this.reg = new Array(8).fill(0); // General-purpose registers
        
        // Special-purpose registers
        this.reg.PC = 0; // Program Counter
        this.reg.IR = 0; // Instruction Register

		this.setupBranchTable();
    }
	
	/**
	 * Sets up the branch table
	 */
	setupBranchTable() {
        let bt = {};
        bt[HLT] = this.HLT;
        bt[ADD] = this.ADD;
        bt[MUL] = this.MUL;
        bt[LDI] = this.LDI;
        bt[PRN] = this.PRN;
        bt[CMP] = this.CMP;
        bt[JEQ] = this.JEQ
        bt[JNE] = this.JNE;
        bt[JMP] = this.JMP;


		this.branchTable = bt;
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
        const _this = this;

        this.clock = setInterval(() => {
            _this.tick();
        }, 1);
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
     * op can be: ADD SUB MUL DIV INC DEC CMP
     */
    alu(op, regA, regB) {
        switch (op) {
            case 'MUL':
                this.reg[regA]  = this.reg[regA] * this.reg[regB];
                break;
                case 'ADD':
                this.reg[regA] = this.reg[regA] + this.reg[regB];
                break;

        }
    }

    /**
     * Advances the CPU one cycle
     */
    tick() {
        // Load the instruction register (IR) from the current PC
        // !!! IMPLEMENT ME
        this.reg.IR =this.ram.read(this.reg.PC);

        // Debugging output
        console.log(`${this.reg.PC}: ${this.reg.IR.toString(2)}`);

        // Based on the value in the Instruction Register, locate the
        // appropriate hander in the branchTable
        // !!! IMPLEMENT ME
        let handler = this.branchTable[this.reg.IR];

        // Check that the handler is defined, halt if not (invalid
        // instruction)
        // !!! IMPLEMENT ME
        if(handler === undefined) {
            console.error('Unknown opcode' + this.reg.IR);
            this.stopClock(); //exit emulator
            return;
        }
        // We need to use call() so we can set the "this" value inside
        let operandA = this.ram.read(this.reg.PC+ 1);
        let operandB = this.ram.read(this.reg.PC+ 2);
        // the handler (otherwise it will be undefined in the handler)
        handler.call(this, operandA, operandB);

        // Increment the PC register to go to the next instruction
        // !!! IMPLEMENT ME
        this.reg.PC += (this.reg.IR >> 6) & 0b00000011;
    }

    // INSTRUCTION HANDLER CODE:
    ADD(regA, regB) {
        this.alu('ADD', regA, regB);
    }
    /**
     * HLT
     */
    HLT() {
        this.stopClock();
    }

/**
     * LDI R,I
     */
    LDI(regNum, value) {
        // !!! IMPLEMENT ME
        this.reg[regNum] = value & 255; // Coerce value to 255 max.
    }

    /**
     * MUL R,R
     */
    MUL(regA, regB) {

        this.alu('MUL', regA, regB);
    }

    /**
     * PRN R
     */
    PRN(regA) {
        // !!! IMPLEMENT ME
        console.log(reg(regA))
    }
    JEQ() {
// JEQ register
// If equal flag is set (true), jump to the address stored in the given register.
// Machine code:
// 01010001 00000rrr

        }

    JMP() {
// JMP register
// Jump to the address stored in the given register.
// Set the PC to the address stored in the given register.
// Machine code:
// 01010000 00000rrr

    }

    CMP() {
// CMP registerA registerB
// Compare the value in two registers.
// If they are equal, set the Equal E flag to 1, otherwise set it to 0.
// If registerA is less than registerB, set the Less-than L flag to 1, otherwise set it to 0.
// If registerA is greater than registerB, set the Greater-than G flag to 1, otherwise set it to 0.
// Machine code:
// 10100000 00000aaa 00000bbb

    }

    JNE() {
// JNE register
// If E flag is clear (false, 0), jump to the address stored in the given register.
// Machine code:
// 01010010 00000rrr

    }
}

module.exports = CPU;
