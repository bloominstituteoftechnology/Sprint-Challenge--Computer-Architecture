

/**
 * LS-8 v2.0 emulator skeleton code
 */

const fs = require('fs');

// Instructions

const HLT = 0b00011011; // Halt CPU
// !!! IMPLEMENT ME

const LDI = 0b00000100; // Load Register Immediate
const MUL = 0b00000101; // Multiply Register

const PRN  = 0b00000110; // Print Register
const PUSH = 0b00001010; // Push Register
const POP  = 0b00001011; // Pop Register
const CALL = 0b00001111; // Call Register
const RET  = 0b00010000; // Return Register
const ADD  = 0b00001100; // Add Register
const JMP  = 0b00010001; // Jump Register
const INT  = 0b00011001;
const IRET = 0b00011010; 
const ST   = 0b00001001; // Store Register
const CMP  = 0b00010110; // Compare Register
const JEQ  = 0b00010011; // Jump Equal Register
const JNE  = 0b00010100; // Jump Not Equal Register


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

        this.reg = new Array(8).fill(0); // General-purpose registers
        
        this.reg[7] =0xf8;

        // Special-purpose registers
        this.reg.PC = 0; // Program Counter
        this.reg.IR = 0; // Instruction Register

        this.flags = {
            interruptsEnabled: true,
        };

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
        bt[ADD] = this.ADD;
        bt[JMP] = this.JMP;
        bt[INT] = this.INT;
        bt[IRET] = this.IRET;
        bt[ST] = this.ST;
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
        }, 200);

        this.timerHandle = setInterval(() => {
            // Trigger timer interrupt
            // set bit 0 of 15 to 1
            this.reg[6] |= 0b00000001;
            console.log("interrupt!") 
        }, 1000);
    }

    /**
     * Stops the clock
     */
    stopClock() {
        clearInterval(this.clock);
        clearInterval(this.timerHandle);
    }

    /**
     * ALU functionality
     * 
     * op can be: ADD SUB MUL DIV INC DEC CMP
     */
    alu(op, regA, regB) {
        let valA = this.reg[regA];
        let valB = this.reg[regB];
        let compare = '';
        if (valA === valB) {
            // console.log(true);
            this.flags.interruptsEnabled = true;
            this.JMP;
            this.JEQ;
        } else {
            // console.log(false);
            this.flags.interruptsEnabled = false;
            this.JMP;
            this.JNE;   
        }
        //valA = this.reg[regA];

        switch (op) {
            case 'MUL':
                // !!! IMPLEMENT ME
                // value_in_regA = valA * valB
                this.reg[regA] = valA * valB & 0b11111111;
                break;
            case 'ADD':
                // !!! IMPLEMENT ME
                // value_in_regA = valA * valB
                this.reg[regA] = valA + valB & 0b11111111;
                break;
            case 'CMP':
                this.reg[regA] = compare & 0b11111111;
                break;
        }
    }

    /**
     * Advances the CPU one cycle
     */
    tick() {
        // Interrupt stuff
        // Check if an interrupt happened
        const maskedInterrupts = this.reg[IS] & this.reg[IM];

        if (this.flags.interruptsEnabled && maskedInterrupts !== 0) {
            for (let i = 0; i <= 7; i++) {
                if (((maskedInterrupts >> i) & 1) === 1) {
                    // Handling interrupt
                    this.flags.interruptsEnabled = false;

                    // Clear the bit in the IS
                    this.reg[IS] &= ~(1 << i);

                    // Push address of next instructin on stack
                    this.reg[7]--; // dec R7 (SP)
                    this.ram.write(this.reg[7], this.reg.PC);
                    
                    for (let j = 0; j <= 7; j++) {
                        this.reg[7]--; // dec R7 (SP)
                        this.ram.write(this.reg[7], this.reg[j]);
                    }
                    
                    // Look up the handler address in the interrupt vector table
                    const vectorTableEntry = 0xF8 + i;
                    const handlerAddress = this.ram.read(vectorTableEntry);

                    // Set PC to handler
                    this.ram.PC = handlerAddress;

                    //console.log('interrupt! ' + i);
                    break;
                }
            }
            this.reg[IS] = 0;
        }


        // Check if an interrupt happened
        // if it did, jump to that interrupt handler
        
        // !!! IMPLEMENT ME

        // Load the instruction register from the current PC
        this.reg.IR = this.ram.read(this.reg.PC);
        // Debugging output
        // console.log(`${this.reg.PC}: ${this.reg.IR.toString(2)}`);

        // Based on the value in the Instruction Register, jump to the
        // appropriate hander in the branchTable
        const handler = this.branchTable[this.reg.IR];

        // Check that the handler is defined, halt if not (invalid
        // instruction)
        if (!handler) {
            console.log(`Invalid instruction at address ${this.reg.PC}: ${this.reg.IR.toString(2)}`);
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
    ADD() {
        // !!! IMPLEMENT ME
        // regA??
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);
        
        this.alu('ADD', regA, regB);
        // Move the PC
        this.reg.PC += 3;
    }
    /**
     * ST R R
     */
    
     ST() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);
        
        this.ram.write(this.reg[regA], this.reg[regB]);
        // Move the PC
        this.reg.PC += 3;
    }

     HLT() {
        // !!! IMPLEMENT ME
        console.log(this.reg);
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
        // Move the PC
        this.reg.PC += 3;
    }

    /**
     * PRN R
     */
    PRN() {
        // !!! IMPLEMENT ME
        const regA = this.ram.read(this.reg.PC + 1);
        console.log(this.reg[regA]);
        
        this.reg.PC += 2;
    }

    /**
     * PUSH R
     */
    PUSH() {
        const regA = this.ram.read(this.reg.PC + 1);

        this.reg[7]--; // dec R7 (SP)
        this.ram.write(this.reg[7], this.reg[regA]);
        console.log(this.reg[regA]);
        this.reg.PC += 2; 
    }
    /**
     * POP R
     */
    POP() {
        const regA = this.ram.read(this.reg.PC + 1);
        const stackVal = this.ram.read(this.reg[7]);
        this.reg[regA] = stackVal;
        this.reg[7]++;

        this.reg.PC += 2;
    }
    /**
     * CALL R
     */
    CALL() {
        const regA = this.ram.read(this.reg.PC + 1);

        // Push address of next instructin on stack
        this.reg[7]--; // dec R7 (SP)
        this.ram.write(this.reg[7], this.reg.PC + 2);
        
        // Jump to the address stored in regA
        this.reg.PC = this.reg[regA]
    }
     /**
      * RET R
      */

      RET() {
        this.reg.PC = this.ram.read(this.reg[7]);
        this.reg[7]++;
      }

      /**
       * JMP R
       */
      JMP() {
        const regA = this.ram.read(this.reg.PC + 1);
        
        // Jump to the address stored in regA
        this.reg.PC = this.reg[regA] 
      }

      IRET() {
        for (let j = 0; j <= 7; j++) {
            this.reg[j] = this.ram.read(this.reg[7]);
            this.reg[7]++; // dec R7 (SP)
        }
        
        // Look up the handler address in the interrupt vector table
        this.reg.PC = this.ram.read(this.reg[7]);
        this.reg[7]++;

        this.flags.interruptsEnabled = true;
      }

      PRA() {
        // const regA = this.ram.read(this.reg.PC + 1);
        
        // console.log(String.fromC);
        
        // this.reg.PC += 2; 
      }

      CMP() {
        // !!! IMPLEMENT ME
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);
        
        this.alu('CMP', regA, regB);
        // Move the PC
        this.reg.PC += 3;

        this.flags.interruptsEnabled = true;
      }

      JEQ() {
        const regA = this.ram.read(this.reg.PC + 1);
        
        // Jump to the address stored in regA
        this.reg.PC = this.reg[regA] 
      }

      JNE() {
        const regA = this.ram.read(this.reg.PC + 1);
        
        // Jump to the address stored in regA
        this.reg.PC = this.reg[regA] 
      }
}

module.exports = CPU;
