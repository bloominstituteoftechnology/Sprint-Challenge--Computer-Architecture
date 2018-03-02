/**
 * LS-8 v2.0 emulator skeleton code
 */

const fs = require('fs');

// Instructions


const ADD = 0b10101000; // ADD two registers together and store the result in registerA.
const AND = 0b10110011;
const CALL = 0b01001000;
const CMP = 0b10100000;
const DEC = 0b01111001;
const DIV = 0b10101011;
const HLT  = 0b00000001; // Halt CPU
const INC = 0b01111000;
const INT = 0b01001010;
const IRET = 0b00001011;
const JEQ = 0b01010001;
const JGT = 0b01010100;
const JLT = 0b01010011;
const JMP = 0b01010000;
const JNE = 0b01010010;
const LD = 0b10011000;
const LDI = 0b10011001; // Set the value of a register to an integer.
const MOD = 0b10101100;
const MUL = 0b10101010; // Multiply two registers together and store the result in registerA.
const NOP = 0b00000000;
const NOT = 0b01110000;
const OR = 0b10110001;
const POP = 0b01001100;
const PRA = 0b01000010;
const PRN = 0b01000011; // Print numeric value stored in the given register
const PUSH = 0b01001101;
const RET = 0b00001001;
const ST = 0b10011010;
const SUB = 0b10101001;
const XOR = 0b10110010;

const SP= 7;
const FL_EQ = 0b00000001;
const FL_GT = 0b00000010;
const FL_LT = 0b00000100;

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
        

        // int the stack pointer
        this.reg[SP] = 0xf3;

        // Special-purpose registers
        this.reg.PC = 0; // Program Counter
        this.reg.IR = 0; // Instruction Register
        this.reg.FL = 1; // Flags

		this.setupBranchTable();
    }
	
	/**
	 * Sets up the branch table
	 */
	setupBranchTable() {
		let bt = {};

       
        bt[ADD] = this.ADD;
        bt[AND] = this.AND;
        bt[CALL] = this.CALL;
        bt[CMP] = this.CMP;
        bt[DEC] = this.DEC;
        bt[DIV] = this.DIV;
        bt[HLT] = this.HLT;
        bt[INC] = this.INC;
        bt[INT] = this.INT;
        bt[IRET] = this.IRET;
        bt[JEQ] = this.JEQ;
        bt[JGT] = this.JGT;
        bt[JLT] = this.JLT;
        bt[JMP] = this.JMP;
        bt[JNE] = this.JNE;
        bt[LD] = this.LD;
        bt[LDI] = this.LDI;
        bt[MOD] = this.MOD;
        bt[MUL] = this.MUL;
        bt[NOP] = this.NOP;
        bt[NOT] = this.NOT;
        bt[OR] = this.OR;
        bt[POP] = this.POP;
        bt[PRA] = this.PRA;
        bt[PRN] = this.PRN;
        bt[PUSH] = this.PUSH;
        bt[RET] = this.RET;
        bt[ST] = this.ST;
        bt[SUB] = this.SUB;
        bt[XOR] = this.XOR;

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
        this.reg.FL = this.reg.FL || flag;
    } else {
        this.reg.FL = this.reg.FL & (~flag);
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
                this.reg[regA] = this.reg[regA] * this.reg[regB];
                break;
            case 'ADD':
               this.reg[regA] = this.reg[regA] + this.reg[regB];
                break;
            case 'AND':
                this.reg[regA] & this.reg[regB];
                break;
            case 'CMP':
             this.setFlag(FL_EQ, this.reg[regA] === this.reg[regB]);
             this.setFlag(FL_LT, this.reg[regA] < this.reg[regB]);
             this.setFlag(FL_GT, this.reg[regA] > this.reg[regB]);
                break;
                
            
        }
    }

    /**
     * Advances the CPU one cycle
     */
    tick() {
        // Load the instruction register (IR) from the current PC
        // !!! IMPLEMENT ME
        this.reg.IR = this.ram.read(this.reg.PC)

        // Debugging output
        //console.log(`${this.reg.PC}: ${this.reg.IR.toString(2)}`);
    

        // Based on the value in the Instruction Register, locate the
        // appropriate hander in the branchTable
        let handler = this.branchTable[this.reg.IR]
        // Check that the handler is defined, halt if not (invalid
        // instruction)
        if( handler === undefined) {
            console.error('Unknown opcode' + this.reg.IR);
            this.stopClock(); //exit emulator
            return;
            
        }

        // We need to use call() so we can set the "this" value inside
        // the handler (otherwise it will be undefined in the handler)
        let operandA = this.ram.read(this.reg.PC + 1)
        let operandB = this.ram.read(this.reg.PC + 2)
        let nextPC = handler.call(this, operandA, operandB);

        // Increment the PC register to go to the next instruction
        // !!! IMPLEMENT ME
        if( nextPC === undefined) {
       this.reg.PC += ((this.reg.IR >> 6) & 0b00000011) + 1;
        } else {
            this.reg.PC= nextPC;
        }
    }

    // INSTRUCTION HANDLER CODE:

    /**
     * ADD R R
     */
    ADD( regA, regB){
    this.alu('ADD',regA,regB);
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
        this.reg[regNum] = value
    }

    /**
     * MUL R,R
     */
    MUL(regA, regB) {
        // !!! IMPLEMENT ME
        // Call the ALU
   this.alu('MUL', regA, regB);
    }

    /**
     * PRN R
     */
    PRN(regA) {
        // !!! IMPLEMENT ME
        console.log(this.reg[regA]);
    }

    AND(regA,regB) {
        this.alu('AND', this.reg[regA], this.reg[regB]);
    }

    NOP() {
        return;
    }
    PUSH(regNum) {
        this.pushHelper(this.reg[regNum]);
    } 
    pushHelper(value) {
       this.reg[SP]--;
       this.ram.write(this.reg[SP], value);
      
    }

    POP(regNum) {
       let value = this.popHelper();
       this.reg[regNum]= value;
    }
    popHelper(value) {
       let val = this.ram.read(this.reg[SP]);
       this.reg[SP]++
       return val;

    }

    CALL(regNum) {
        // Push next address on stack
       this.pushHelper(this.reg.PC +2);
        //set PC to value in regNum
        return this.reg[regNum];

    }

    RET() {
        const next =this.popHelper();
        return next;

    }

    CMP(regA, regB) {
        this.alu('CMP', regA, regB)
}
    JMP(regNum) {
        return( this.reg[regNum])

    }

    JEQ(regNum){
        if (FL_EQ ) {
            return(this.reg[regNum]);
        } 
    }

    JGT(regNum) {
        if(FL_GT ) {
            return (this.reg[regNum]);
        }

    }

    JLT(regNum){
        if ( FL_LT ) {
            return (this.reg[regNum]);
        }
    }

    JNE(regNum) {
        if (!FL_EQ ) {
            return(this.reg[regNum]);
        }
    }
    
}

module.exports = CPU;
