/**
 * LS-8 v2.0 emulator skeleton code
 */

const fs = require('fs');

// Instructions
// ADD POP PUSH DIV
const HLT  = 0b00011011; // Halt CPU
// !!! IMPLEMENT ME
const LDI =  0b00000100;  // load register imediate
const ADD = 0b00001100; // add
const MUL =  0b00000101;    // multiply
const DIV =  0b00001110;   // divide r1 by r2
const PRN =  0b00000110;     // print
const PUSH = 0b00001010;  // pushit
const POP =  0b00001011;  // pop
const RET =  0b00010000; // return 
const CALL = 0b00001111;  // call register

//const POP = 0b

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
        
        this.reg[7] = 0xf8;

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
        // !!! IMPLEMENT ME
        bt[LDI] = this.LDI;
        bt[MUL] = this.MUL;
        bt[PRN] = this.PRN;
        bt[DIV] = this.DIV;
        bt[ADD] = this.ADD;
        bt[PUSH] = this.PUSH;
        bt[RET] = this.RET;
        bt[CALL] = this.CALL;
        
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
        let valA = this.reg[regA];
        let valB = this.reg[regB];

        switch (op) {
            case 'MUL':
                this.reg[regA] = (valA * valB) & 255;
        
                // !!! IMPLEMENT ME
            //     return regA = regA * regB ;
            // case 'ADD':
            //     return regA = regA + regB;
           // case 'SUB':
           //     return regA 
                break;
            case 'DIV':
                //console.log(valB);
                if (valB === 0) {
                    console.log("Hey, Fuckface. Stop dividing by zero...")
                     this.stopClock();
                     
                    break;
                }
                this.reg[regA] = (valA / valB) & 255;
                break;
            case 'ADD':
                this.reg[regA] = (valA + valB) & 255;
                break;
            case 'PUSH':
                console.log("why is  this here?");
                break;
        }
    }

    /**
     * Advances the CPU one cycle
     */
    tick() {
        // !!! IMPLEMENT ME
        this.reg.IR = this.ram.read(this.reg.PC);
        // Load the instruction register from the current PC

        // Debugging output
        //console.log(`${this.reg.PC}: ${this.reg.IR.toString(2)}`);

        // Based on the value in the Instruction Register, jump to the
        // appropriate hander in the branchTable
        const handler = this.branchTable[this.reg.IR];
        // Check that the handler is defined, halt if not (invalid
        // instruction)
        if(!handler) {
            console.log("error on CPU handedr ln 114");
            //console.error(`Invalid inst at address ${ this.reg.PC }: ${this.reg.IR.toString(2)} `);
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
        // !!! IMPLEMENT ME
        this.stopClock();
    }

    /**
     * LDI R,I
     */
    LDI() {
        // !!! IMPLEMENT ME
        const regA = this.ram.read(this.reg.PC + 1);
        const val = this.ram.read(this.reg.PC + 2);

        this.reg[regA] = val;
        // move the PC
        this.reg.PC += 3;
    }

    /**
     * MUL R,R
     */
    MUL() {
        // !!! IMPLEMENT ME
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);

        this.alu('MUL', regA, regB);
        // move the PC
        this.reg.PC += 3;
    }
    
    DIV() {
        // !!! IMPLEMENT ME
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);

        this.alu('DIV', regA, regB);
        // move the PC
        this.reg.PC += 3;
    }
    ADD() {
        // !!! IMPLEMENT ME
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);

        this.alu('ADD', regA, regB);
        // move the PC
        this.reg.PC += 3;
    }
    PUSH() {
        // !!! IMPLEMENT ME
         
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
    /**
     * PRN R
     */
    PRN() {
        // !!! IMPLEMENT ME
      //  this.reg.PC += 2;

    
            const regA = this.ram.read(this.reg.PC + 1);
    
            console.log(this.reg[regA]);

            this.reg.PC += 2;
            
        
    }
    CALL() {
        // calls subroutine at address stored in register
        //before call is made address of next instruction to execute is pushed in  the stack
        const regA = this.ram.read(this.reg.PC + 1);
        
        this.reg[7]--;
        this.ram.write(this.reg[7], this.reg.PC + 2);

        this.reg.PC = this.reg[regA];
    }
    RET() {
        // get return adress of stack and then set program counter to it. 
        
    }
  
}

module.exports = CPU;
