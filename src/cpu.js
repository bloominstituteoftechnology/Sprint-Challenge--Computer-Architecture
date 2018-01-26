/**
 * LS-8 v2.0 emulator skeleton code
 */

const fs = require('fs');

// Instructions

const HLT = 0b00011011; // Halt CPU
// !!! IMPLEMENT ME
// LDI Load Register Immediate
const LDI  = 0b00000100;
const MUL  = 0b00000101;
const PRN  = 0b00000110;
const PUSH = 0b00001010; // push register
const POP  = 0b00001011; // pop register
const CALL = 0b00001111;
const RET  = 0b00010000;
const ADD  = 0b00001100;
const ST   = 0b00001001;
const PRA  = 0b00000111;
const IRET = 0b00011010;
const LD   = 0b00010010; // Load R R
const JMP  = 0b00010001;
const JEQ  = 0b00010011;
const JNE  = 0b00010100;
const CMP  = 0b00010110; 
const I0Vector = 0xf8;
const timerInterrupt = 1;

const IM = 5;
const IS = 6;
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

        this.reg = new Array(8).fill(0); // General-purpose registers
        this.reg[SP] = I0Vector;
        this.reg[7] = 0xf8;
        this.reg.PC = 0; // Program Counter 
        this.reg.IR = 0; // Instruction Register
        
        this.flags = {
            interruptsEnabled: true,
            isEqual: false,
        }
        this.fanSpeed = 2;

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
        bt[PUSH] = this.PUSH;
        bt[ADD] = this.ADD;
        bt[POP] = this.POP;
        bt[CALL] = this.CALL;
        bt[RET] = this.RET;
        bt[LD] = this.LD;
        bt[ST] = this.ST;
        bt[PRA] = this.PRA;
        bt[IRET] = this.IRET;
        bt[JMP] = this.JMP;
        bt[JEQ] = this.JEQ;
        bt[JNE] = this.JNE;
        bt[CMP] = this.CMP;

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
        /*
        this.timerHandle = setInterval(() => {
            
            this.raiseInterrupt(timerInterrupt)
        }, 1000);
        */
    }
    /**
     * Stops the clock
     */
    stopClock() {
        clearInterval(this.clock);
        clearInterval(this.timerHandle);
    }
    raiseInterrupt(n) {
        this.reg[IS] |= n;
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
                this.reg[regA] = (valA * valB) & 0b11111111;
                break;
            case 'ADD':
                this.reg[regA] = (valA + valB) & 0b11111111;
                break;
        }
    }
    /**
     * Advances the CPU one cycle
     */
    tick() {
        /*
        const maskedInterrupts = this.reg[IS] & this.reg[IM];
        if(this.flags.interruptsEnabled && maskedInterrupts !== 0 ){
            for (let i = 0; i <= 7;  i++){
            if(((maskedInterrupts >> i ) & 1) === 1){
                this.flags.interruptsEnabled = false;
                this.reg[IS] &= ~(1 << i);
                this.reg[7]--;
                this.ram.write(this.reg[7], this.reg.PC);

                for (let j = 0; j <= 7; j++) {
                    this.reg[SP]--;
                    this.ram.write(this.reg[SP], this.reg[j])
                }

                const vectorTableEntry = 0xf8 + i;
                const handlerAddress = this.ram.read(vectorTableEntry);
                this.reg.PC = handlerAddress;
                break;
                }
            }
        }
        */
        this.reg.IR = this.ram.read(this.reg.PC);
        const handler = this.branchTable[this.reg.IR];
        if (!handler) {
            console.log(`Invalid instruction at address ${this.reg.PC}: ${this.reg.IR.toString(2)}`);
            this.stopClock();
            return;
        }
        handler.call(this);
    }
    // INSTRUCTION HANDLER CODE:
    HLT() {
        this.stopClock();
    }
    LDI() {
        const regA = this.ram.read(this.reg.PC + 1);
        const val = this.ram.read(this.reg.PC + 2);
        this.reg[regA] = val;
        this.reg.PC += 3;
    }
    MUL() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);
        this.alu('MUL', regA, regB);
        this.reg.PC +=3;
    }
    ADD() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);
        this.alu('ADD', regA, regB);
        this.reg.PC +=3;   
    }
    PRN() {
        const regA = this.ram.read(this.reg.PC  + 1)
        console.log(this.reg[regA])
        this.reg.PC +=2;
    }
    PUSH() {
        this.reg[7]-- 
        const regA = this.ram.read(this.reg.PC + 1);
        console.log(regA)
        this.ram.write(this.reg[7], this.reg[regA]);
        this.reg.PC +=2;
    }
    POP() {
        const regA = this.ram.read(this.reg.PC + 1);
        console.log(regA)
        const stackValue = this.ram.read(this.reg[7]);
        this.reg[regA] = stackValue;
        this.reg[7]++;
        this.reg.PC +=2;
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
    ST() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);
        this.ram.write(this.reg[regA], this.reg[regB])
        this.reg.PC +=3;
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
          const regA = this.ram.read(this.reg.PC  + 1)
          console.log(String.fromCharCode(this.reg[regA]));
          this.reg.PC +=2;
    }
    LD() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);
        this.reg[regA] = this.ram.read(this.reg[regB]);
        this.reg.PC += 3;
    }
    JMP() {
        const regA = this.ram.read(this.reg.PC + 1);
        this.reg.PC = this.reg[regA];
    }
    CMP() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);
        if(this.reg[regA] === this.reg[regB]) {
            this.flags.isEqual = true;
        } 
        this.flags.isEqual = false;
    }
    JEQ() {
        if(this.flags.isEqual === true) {
            const regA = this.ram.read(this.reg.PC + 1);
            this.reg.PC = this.reg[regA];
        }
    }
    JNE() {
        if(this.flags.isEqual === false) {
            const regA = this.ram.read(this.reg.PC + 1);
            this.reg.PC = this.reg[regA];
        }
    }
}

module.exports = CPU;
