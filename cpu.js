/**
 * LS-8 v2.0 emulator skeleton code
 */

const fs = require('fs');

// Stack Creation


// Instructions

const SP = 7; // STACK POINTER          | Stack starts at F4 memory location: 244.
const IS = 6; // INTERRUPT STATUS       | Reserved Register Location for Interrupt Status
const IM = 5; // INTERRUPT MASK         | Reserved Register Location for Interrupt Mask

const FL_EQ = 0b00000001; // FLAG X = Y | Equality Flag: EQUAL.
const FL_GT = 0b00000010; // FLAG X > Y | Equality Flag: Greater Than.
const FL_LT = 0b00000100; // FLAG X < Y | Equality Flag: Less Than.

const HLT = 0b00000001; // HLT          | Halt CPU.
const LDI = 0b10011001; // LDI REG      | Save the value given inside a register.
const MUL = 0b10101010; // MUL REG REG  | Multiply two registers together and store the result in registerA.
const PRN = 0b01000011; // PRN REG      | Print numeric value stored in the given register.
const ADD = 0b10101000; // ADD REG REG  | Adds two registers and saves the result in the first.
const AND = 0b10110011; // AND REG REG  | Bitwise-AND registerA and registerB, then store the result in registerA.
const NOP = 0b00000000; // NOP          | No operation. Do nothing for this instruction.
const NOT = 0b01110000; // NOT REG      | Performs a bitwise-NOT on the value in the provided register.
const PUSH = 0b01001101; // PUSH REG    | Decrement the SP, Copy Reg Value to location pointed by the new SP.
const POP = 0b00001001; // POP R        | Copy Value pointed by SP to Reg, Increment the SP.
const CMP = 0b10100000; // CMP REG REG  | Compare the value in two registers. Sets Appropriate Flag in reg.FL bitwise.
// --------- NEED TO IMPLEMENT
const JMP = 0b01010000; // JMP REG      | Jump to the address stored in the given register.
const JEQ = 0b01010001; // JEQ REG      | If equal flag is set (true), jump to the address stored in the given register.
const JNE = 0b01010010; // JNE REG      | If E flag is clear (false, 0), jump to the address stored in the given register.

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
        this.reg.PC = 0; // Program Counter.
        this.reg.IR = 0; // Instruction Register.
        this.reg.FL = 0; // Comparison Flag FL bits: 00000LGE.
        this.reg[SP] = 0xF4; //INITIALIZE Stack Pointing to Memory.
        this.reg[IS] = 0; // INITIALIZE Interrupt Status on Register.
        this.reg[IM] = 0; // INITIALIZE Interrupt Mask on Register.

        this.setupBranchTable();
    }

	/**
	 * Sets up the branch table
	 */
    setupBranchTable() {
        let bt = {};

        bt[HLT] = this.HLT;
        bt[LDI] = this.LDI;
        bt[MUL] = this.MUL;
        bt[PRN] = this.PRN;
        bt[ADD] = this.ADD;
        bt[AND] = this.AND;
        bt[NOP] = this.NOP;
        bt[NOT] = this.NOT;
        bt[CMP] = this.CMP;
        bt[PUSH] = this.PUSH;
        bt[POP] = this.POP;
        // --------- NEED TO IMPLEMENT
        bt[JMP] = this.JMP;
        bt[JEQ] = this.JEQ;
        bt[JNE] = this.JNE;


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

    setFlag(flag, value) {
        if (value === true) {
            this.reg.FL = this.reg.FL | flag;
        } else {
            this.reg.FL = this.reg.FL & (~flag); // The Inverse of the Flag is obtained using ~
        }
    }

    /**
     * ALU functionality
     * 
     * op can be: ADD SUB MUL DIV INC DEC CMP
     */
    alu(op, regA, regB) {
        switch (op) {
            case 'MUL':
                this.reg[regA] = (this.reg[regA] * this.reg[regB]) & 255;
                break;
            case 'ADD':
                this.reg[regA] = (this.reg[regA] + this.reg[regB]) & 255;
                break;
            case 'AND':
                this.reg[regA] = (this.reg[regA] & this.reg[regB]) & 255;
                break;
            case 'NOT':
                this.reg[regA] = (~this.reg[regA]) & 255;
                break;
            case 'CMP':
                this.setFlag(FL_EQ, this.reg[regA] === this.reg[regB]);
                break;
        }
    }

    /**
     * Advances the CPU one cycle
     */
    tick() {
        // Load the instruction register (OR) from the current PC
        this.reg.IR = this.ram.read(this.reg.PC) // Use the PC counter to load the appropriate Instruction from Ram into the Register.

        // Debugging output
        // console.log(`${this.reg.PC}: ${this.reg.IR.toString(2)}`);

        // Based on the value in the Instruction Register, locate the
        // appropriate hander in the branchTable
        let handler = this.branchTable[this.reg.IR];

        // Check that the handler is defined, halt if not (invalid
        // instruction)
        if (handler === undefined) {
            console.log(`Unknown code: ${this.reg.IR}`);
            this.stopClock(); // Exit with Error
            return;
        }

        // We need to use call() so we can set the "this" value inside
        // the handler (otherwise it will be undefined in the handler)
        let operandA = this.ram.read(this.reg.PC + 1); // The first Operand is one line after the Instruction.
        let operandB = this.ram.read(this.reg.PC + 2); // The second Operand is two lines after the Instruction.
        handler.call(this, operandA, operandB);

        // Increment the PC register to go to the next instruction
        //this.reg.PC += ((this.reg.IR >> 6) & 0b00000011) + 1;
    }

    // INSTRUCTION HANDLER CODE:

    HLT() {
        this.stopClock();
    }

    LDI(regNum, value) {
        this.reg[regNum] = value & 255; // Coerce value to 255 max.
        this.reg.PC++;
        this.reg.PC++;
        this.reg.PC++;
    }

    MUL(regA, regB) {
        this.alu('MUL', regA, regB);
        this.reg.PC++;
        this.reg.PC++;
        this.reg.PC++;
    }

    PRN(regA) {
        console.log(this.reg[regA])
        this.reg.PC++;
        this.reg.PC++;
    }

    ADD(regA, regB) {
        this.alu('ADD', regA, regB);
        this.reg.PC++;
        this.reg.PC++;
        this.reg.PC++;
    }

    AND(regA, regB) {
        this.alu('AND', regA, regB);
        this.reg.PC++;
        this.reg.PC++;
        this.reg.PC++;
    }

    NOT(regA) {
        this.alu('NOT', regA);
        this.reg.PC++;
        this.reg.PC++;
    }

    NOP() {
        this.reg.PC++;
        return;
    }

    CMP(regA, regB) {
        this.alu('CMP', regA, regB);
        this.reg.PC++;
        this.reg.PC++;
        this.reg.PC++;
    }

    PUSH(regA) {
        // Decrease the Stack
        this.reg[SP] = this.reg[SP] - 1;
        this.ram.write(this.reg[SP], this.reg[regA]);
        this.reg.PC++;
        this.reg.PC++;
    }

    POP(regA) {
        this.reg[regA] = this.ram.read(this.reg[SP]);
        this.reg[SP] = this.reg[SP] + 1;
        this.reg.PC++;
        this.reg.PC++;
    }

    JMP(regA) {
        this.reg.PC = this.reg[regA];
    }

    JEQ(regA) {
        const bitComp = this.reg.FL & FL_EQ; // 1 if True 0 if False
        this.reg.PC = bitComp ? this.reg[regA] : this.reg.PC + 2;
    }

    JNE(regA) {
        const bitComp = this.reg.FL & FL_EQ; // 1 if True 0 if False
        this.reg.PC = bitComp ? this.reg.PC + 2 : this.reg[regA];
    }
}

module.exports = CPU;
