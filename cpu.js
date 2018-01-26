// import { setInterval } from 'timers';

/**
 * LS-8 v2.0 emulator skeleton code
 */

const fs = require('fs');

// Instructions

const HLT  = 0b00011011; // Halt CPU
const LDI  = 0b00000100; // Load Register Immediate
const MUL  = 0b00000101; // Multiply Register Register
const PRN  = 0b00000110; // print Register Register
const PUSH = 0b00001010;
const POP = 0b00001011; // Push Register
const CALL = 0b00001111;
const RET = 0b00010000;
const JMP = 0b00010001;
const ADD = 0b00001100;
const INT = 0b00011001;
const ST = 0b00001001;
const IRET = 0b00011010;
const PRA = 0b00000111;
const CMP = 0b00010110;
const JEQ = 0b00010011;
const JNE = 0b00010100;

const IM = 5;
const IS = 6;

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
        this.reg[7] = 0xf8;
        // Special-purpose registers
        this.reg.PC = 0; // Program Counter
        this.reg.IR = 0; // Instruction Register

        this.flags = {
            interruptsEnabled: true,
            overflow: false,
            equal: false
        }

		this.setupBranchTable();
    }
	
	/**
	 * Sets up the branch table
	 */
	setupBranchTable() {
		let bt = {};

        bt[HLT] = this.HLT;
        bt[MUL] = this.MUL;
        bt[LDI] = this.LDI;
        bt[PRN] = this.PRN;
        bt[PUSH] = this.PUSH;
        bt[POP] = this.POP;
        bt[CALL] = this.CALL;
        bt[RET] = this.RET;
        bt[JMP] = this.JMP;
        bt[ADD] = this.ADD;
        bt[INT] = this.INT;
        bt[ST] = this.ST;
        bt[IRET] = this.IRET;
        bt[PRA] = this.PRA;
        bt[CMP] = this.CMP;
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

        this.timerHandle = setInterval(() => {
            this.reg[IS] |= 0b00000001
        }, 1000);
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
        let valA = this.reg[regA];
        let valB = this.reg[regB];

        switch (op) {
            case 'MUL':
                this.reg[regA] = (valA * valB) & 255;
                break;
            case 'ADD':
                this.reg[regA] = (valA + valB) & 255;
                break;
            case 'CMP':
                valA === valB ? this.flags.equal = true : this.flags.equal = false;
                break;
        }
    }

    /**
     * Advances the CPU one cycle
     */
    tick() {
        // Interrupt stuff
        const maskedInterrupts = this.reg[IS] & this.reg[IM];
        
        if (this.flags.interruptsEnabled && maskedInterrupts !== 0) {
            for (let i = 0; i <= 7; i++) {
                if (((maskedInterrupts >> i) & 1) === 1) {
                    this.flags.interruptsEnabled = false;

                    this.reg[IS] &= ~(1 << i);

                    this.reg[7]--;
                    this.ram.write(this.reg[7], this.reg.PC);

                    for (let j = 0; j <= 7; j++) {
                        this.reg[7]--;
                        this.ram.write(this.reg[7], this.reg[j]);
                    }

                    const vectorTableEntry = 0xf8 + i;
                    const handlerAddress = this.ram.read(vectorTableEntry);

                    this.reg.PC = handlerAddress;

                    break;
                }
            }
        }
        
        // Load the instruction register from the memory address pointed to by
        // the PC
        this.reg.IR = this.ram.read(this.reg.PC);

        // Debugging output
        // console.log(`${this.reg.PC}: ${this.reg.IR.toString(2)}`);

        // Based on the value in the Instruction Register, jump to the
        // appropriate hander in the branchTable
        const handler = this.branchTable[this.reg.IR];

        // Check that the handler is defined, halt if not (invalid
        // instruction)
        if (!handler) {
            console.error(`Invalid instruction at address ${this.reg.PC}: ${this.reg.IR.toString(2)}`);
            this.stopClock();
            return;
        }

        // We need to use call() so we can set the "this" value inside
        // the handler (otherwise it will be undefined in the handler)
        handler.call(this);
    }

    // INSTRUCTION HANDLER CODE:

    /**
     * HLT
     */
    HLT() {
        this.stopClock();
    }

    /**
     * LDI R,I
     */
    LDI() {
        const regA = this.ram.read(this.reg.PC + 1);
        const val = this.ram.read(this.reg.PC + 2);

        this.reg[regA] = val;

        this.reg.PC += 3;
    }

    /**
     * MUL R,R
     */
    MUL() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);

        this.alu('MUL', regA, regB);

        // Move the PC
        this.reg.PC += 3;
    }

    /**
     * PRN R
     */
    PRN() {
        const regA = this.ram.read(this.reg.PC + 1);

        console.log(this.reg[regA]);

        this.reg.PC += 2;
    }

    PUSH() {
        const regA = this.ram.read(this.reg.PC + 1);
        this.reg[7]--;
        this.ram.write(this.reg[7], this.reg[regA]);
        this.reg.PC += 2;
    }

    POP() {
        const regA = this.ram.read(this.reg.PC + 1);
        const stackVal = this.ram.read(this.reg[7]);
        this.reg[regA] = stackVal;
        this.reg[7]++;
        this.reg.PC += 2;
    }

    CALL() {
        const regA = this.ram.read(this.reg.PC + 1);

        this.reg[7]--;

        this.ram.write(this.reg[7], this.reg.PC + 2);
        this.reg.PC = this.reg[regA];
    }

    RET() {
        this.reg.PC = this.ram.read(this.reg[7]);
        this.reg[7]++;
    }

    JMP() {
        const regA = this.ram.read(this.reg.PC + 1);

        this.reg.PC = this.reg[regA];
    }

    ADD() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);

        this.alu('ADD', regA, regB);

        this.reg.PC += 3;
    }

    ST() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);

        this.ram.write(this.reg[regA], this.reg[regB]);

        this.reg.PC += 3;
    }
    
    IRET() {
        for (let j = 7; j >= 0; j--) {
            this.reg[j] = this.ram.read(this.reg[7]);
            this.reg[7]++;
        }
        
        this.reg.PC = this.ram.read(this.reg[7]);
        this.reg[7]++;
        
        this.flags.interruptsEnabled = true;
    }

    PRA() {
        const regA = this.ram.read(this.reg.PC + 1);

        console.log(String.fromCharCode(this.reg[regA]));

        this.reg.PC += 2;
    }

    CMP() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);

        this.alu('CMP', regA, regB);

        this.reg.PC += 3;
    }

    JEQ() {
        if (this.flags.equal) {
            const regA = this.ram.read(this.reg.PC + 1);

            this.reg.PC = this.reg[regA];
        } else {
            this.reg.PC += 2;
        }
    }

    JNE() {
        if (!this.flags.equal) {
            const regA = this.ram.read(this.reg.PC + 1);

            this.reg.PC = this.reg[regA];
        } else {
            this.reg.PC += 2;
        }
    }
}

module.exports = CPU;
