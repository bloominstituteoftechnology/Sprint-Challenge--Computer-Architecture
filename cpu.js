/**
 * LS-8 v2.0 emulator skeleton code
 */

const fs = require('fs');

// Instructions

const HLT  = 0b00011011; // Halt CPU
const LDI = 0b00000100; // Set the value of a register.
const ADD = 0b00001100; // ADD
const SUB = 0b00001101; // SUB
const MUL = 0b00000101; // MUL
const DIV = 0b00001110; // DIV
const CMP = 0b00010110; // Compare the value in two registers.
const PRN = 0b00000110; // Print
const PUSH = 0b00001010; // Push Register
const POP = 0b00001011; // Pop Register
const CALL = 0b00001111; // Calls a subroutine at the address stored in the register.
const RET = 0b00010000; // Return from subroutine.
const JMP = 0b00010001; // Jump to the address stored in the given register.
const JEQ = 0b00010011; // If equal flag is set (true), jump to the address stored in the given register.
const JNE = 0b00010100; // If equal flag is clear (false), jump to the address stored in the given register.
const ST = 0b00001001; // Store R R
const LD = 0b0010010; // Loads registerA with the value at the address stored in registerB.
const IRET = 0b00011010; // Return from an interrupt handler.
const PRA = 0b00000111; // Print alpha character value stored in the given register.

const timerInterrupt = 1;
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
        
        this.reg[7] = 0xF8;

        // Special-purpose registers
        this.reg.PC = 0; // Program Counter
        this.reg.IR = 0; // Instruction Register

        this.flags = {
            interruptsEnabled: true
        };

		this.setupBranchTable();
    }
	
	/**
	 * Sets up the branch table
	 */
	setupBranchTable() {
		let bt = {};

        bt[HLT] = this.HLT;
        bt[LDI] = this.LDI;
        bt[ADD] = this.ADD;
        bt[SUB] = this.SUB;
        bt[MUL] = this.MUL;
        bt[DIV] = this.DIV;
        bt[CMP] = this.CMP;
        bt[PRN] = this.PRN;
        bt[PUSH] = this.PUSH;
        bt[POP] = this.POP;
        bt[RET] = this.RET;
        bt[JMP] = this.JMP;
        bt[JEQ] = this.JEQ;
        bt[JNE] = this.JNE;
        bt[CALL] = this.CALL;
        bt[ST] = this.ST;
        bt[LD] = this.LD;
        bt[IRET] = this.IRET;
        bt[PRA] = this.PRA;

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
            // Trigger timer interrupt
            // set bit 0 to IS to 1
            // this.reg[6] |= 0b00000001;
            this.raiseInterrupt(timerInterrupt);
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
     * Raise an interrupt
     */
    raiseInterrupt(n) {
        this.reg[6] |= n;
    }


    /**
     * ALU functionality
     * 
     * op can be: ADD SUB MUL DIV INC DEC CMP
     */
    alu(op, regA, regB) {
        switch (op) {
            case 'ADD':
                this.reg[regA] += this.reg[regB] & 0b11111111;
                break;
            case 'SUB':
                this.reg[regA] -= this.reg[regB] & 0b11111111;
                break;
            case 'MUL':
                this.reg[regA] *= this.reg[regB] & 0b11111111;
                break;
            case 'DIV':
                if (this.reg[regB] === 0) {
                    console.log("error: can't divide by zero");
                    this.stopClock();
                }
                this.reg[regA] /= this.reg[regB];
                break;
            case 'CMP':
                if (this.reg[regA] === this.reg[regB]) {
                    this.flags.equal = true;
                } else {
                    this.flags.equal = false;
                }
                break;
        }
    }

    /**
     * Advances the CPU one cycle
     */
    tick() {
        const maskedInterrupts = this.reg[6] & this.reg[5];

        // Check if an interrupt happened
        // if it did, jump to that interrupt handler
        if (this.flags.interruptsEnabled && maskedInterrupts !== 0) {
            for (let i = 0; i <= 7; i++) {
                if (((maskedInterrupts >> i) & 1) === 1) {
                    // Handling interrupt
                    this.flags.interruptsEnabled = false;

                    // Clear the bit in the IS
                    this.reg[6] &= ~(1 << i);

                    // push PC on stack
                    this.reg[7]--;
                    this.ram.write(this.reg[7], this.reg.PC);

                    // push remaining registers on stack
                    for (let j = 0; j <= 7; j++) {
                        this.reg[7]--;
                        this.ram.write(this.reg[7], this.reg[j]);
                    }

                    // look up the handler address in the interrupt vector table
                    const vectorTableEntry = 0xf8 + i;
                    const handlerAddress = this.ram.read(vectorTableEntry);

                    // Set PC to handler
                    this.reg.PC = handlerAddress;

                    console.log('interrupt! ' + i);
                    break;
                }
            }
        }
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
            console.error('Invalid instructions at address ' + this.reg.PC + ' ' + this.reg.IR);
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
        const val = this.ram.read(this.reg.PC + 2); // immediate
        this.reg[regA] = val;
        this.reg.PC += 3;
    }

    /**
     * LD R,I
     */
    LD() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);

        this.reg[regA] = this.ram.read(this.reg[regB]);

        this.reg.PC += 3;
    }

    /**
     * SUB R,R
     */
    ADD() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);

        this.alu('ADD', regA, regB);
        
        this.reg.PC += 3;
    }

     /**
     * SUB R,R
     */
    SUB() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);

        this.alu('SUB', regA, regB);
        
        this.reg.PC += 3;
    }

    /**
     * MUL R,R
     */
    MUL() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);

        this.alu('MUL', regA, regB);
        
        this.reg.PC += 3;
    }

    /**
     * DIV R,R
     */
    DIV() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);

        this.alu('DIV', regA, regB);
        
        this.reg.PC += 3;
    }

    /**
     * CMP R,R
     */
    CMP() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);

        this.alu('CMP', regA, regB);
        
        this.reg.PC += 3;
    }

    /**
     * PRN R
     */
    PRN() {
        console.log(this.reg[this.ram.read(this.reg.PC + 1)]);
        this.reg.PC += 2;
    }

    /**
     * PRA R
     */
    PRA(reg) {
        process.stdout.write(String.fromCharCode(this.reg[reg]));
        this.reg.PC += 2;
    }

    /**
     * PUSH R
     */
    PUSH() {
        const regA = this.ram.read(this.reg.PC + 1);
        this.reg[7]--; // dec r7;
        this.ram.write(this.reg[7], this.reg[regA]);
        this.reg.PC += 2;
    }

    /**
     * POP R
     */
    POP() {
        this.reg[this.ram.read(this.reg.PC + 1)] = this.ram.read(this.reg[7]);
        this.reg[7]++;
        this.reg.PC += 2;
    }

    /**
     * ST R R
     */
    ST() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);

        this.ram.write(this.reg[regA], this.reg[regB]);

        this.reg.PC += 3;
    }

    /**
     * IRET R R
     */
    IRET() {
        // Pop remaining registers on stack
        for (let j = 7; j >= 0; j--) {
            this.reg[j] = this.ram.read(this.reg[7]);
            this.reg[7]++;
        }

        // pop PC off stack
        this.reg.PC = this.ram.read(this.reg[7]);
        this.reg[7]++;

        // enable interrupts
        this.flags.interruptsEnabled = true;
    }
    
    /**
     * CALL R
     */
    CALL() {
        const regA = this.ram.read(this.reg.PC + 1);
        this.reg[7]--;
        this.ram.write(this.reg[7], this.reg.PC + 2);
        this.reg.PC = this.reg[regA];
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
        this.reg.PC = this.reg[regA];
    }

    /**
     * JEQ R
     */
    JEQ() {
        if (this.flags.equal) {
            const regA = this.ram.read(this.reg.PC + 1);
            this.reg.PC = this.reg[regA];
        } else {
            this.reg.PC += 2;
        }
    }

    /**
     * JNE R
     */
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
