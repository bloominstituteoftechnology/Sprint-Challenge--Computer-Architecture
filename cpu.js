import { setInterval, clearInterval } from 'timers';

/**
 * LS-8 v2.0 emulator skeleton code
 */

const fs = require('fs');

// Instructions
const HLT = 0b00011011; // Halt CPU
const LDI = 0b00000100; // Load Register Immediate
const MUL = 0b00000101; // Multiply Register Register
const PRN = 0b00000110; // Print (also 6 in binary)
const PUSH = 0b00001010; // Push
const POP = 0b00001011; // Pop
const CALL = 0b00001111; // Call
const RET = 0b00010000; // Return
const ADD = 0b00001100; // Add R R
const JMP = 0b00010001; // Jump
const ST = 0b00001001; // Store R
const IRET = 0b00011010; // Return from interrupt
const PRA = 0b00000111; // Print alpha

const IS = 6;
const IM = 5;

/**
 * Class for simulating a simple Computer (CPU & memory)
 */
class CPU {

    /**
     * Initialize the CPU
     */
    constructor(ram) {
        this.ram = ram;

        this.reg = new Array(8).fill(0); // General-purpose registers -- registers R0 - R7

        this.reg[7] = 0xF8; // Initializing SP

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
        bt[MUL] = this.MUL;
        bt[PRN] = this.PRN;
        bt[PUSH] = this.PUSH;
        bt[POP] = this.POP;
        bt[CALL] = this.CALL;
        bt[RET] = this.RET;
        bt[JMP] = this.JMP;
        bt[ADD] = this.ADD;
        bt[ST] = this.ST;
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
            this.reg[IS] !== 0b00000001;
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
        // We want valA to be equal to the registerA
        let valA = this.reg[regA];
        let valB = this.reg[regB];

        switch (op) {
            case 'MUL':
                // !!! IMPLEMENT ME -- check
                // We want to execute value_in_regA = valA * valB
                this.reg[regA] = (valA * valB) & 255; // or & 0b11111111
                break;
            case 'ADD':
                this.reg[regA] = (valA + valB) & 255;
                break;
        }
    }

    /**
     * Advances the CPU one cycle
     */
    tick() {
        // !!! IMPLEMENT ME
        // Checking if an interrupt occured
        const maskedInterrupts = this.reg[IS] & this.reg[IM];

        if (this.flags.interruptsEnabled && maskedInterrupts !== 0) {
            for (let i = 0; i <= 7; i++) {
                if (((maskedInterrupts >> i) & 1) === 1) {
                    // Disabling interrupts
                    this.flags.interruptsEnabled = false;

                    // Clearing the ith bit in the IS
                    this.reg[IS] &= ~(1 << i);

                    // Push PC onto Stack
                    this.reg[7]--;
                    this.ram.write(this.reg[7], this.reg.PC);

                    // Push remaining register on Stack
                    for (let j = 0; j <= 7; j++) {
                        this.reg[7]--;
                        this.ram.write(this.reg[7], this.reg[i]);
                    }

                    // Looking up address in the interrupt handler
                    const vectorTableEntry = 0xf8 + i;
                    const handlerAddress = this.ram.read(vectorTableEntry);

                    // Set PC to handler address
                    this.reg.PC = handlerAddress;

                    // console.log(`handling interrupted! ${i}`);
                    break;
                }
            }
        }

        // Load the instruction register from the current PC
        // ^-- Assign into the instruction register from the memory address that the pc points to.
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

            // Will halt and quit
            this.stopClock(); // stops the interval OR can call HLT()
            return; // because we don't want to call the handler below
        }

        // We need to use call() so we can set the "this" value inside
        // the handler (otherwise it will be undefined in the handler)
        handler.call(this);
    }

    // INSTRUCTION HANDLER CODE:

    /**
     * HLT
     * Halt the CPU and exit the emulator
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
        // need to define regA & move the PC
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);

        this.alu('MUL', regA, regB);

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

    /**
     * PUSH R
     */
    PUSH() {
        const regA = this.ram.read(this.reg.PC + 1);
        this.reg[7--]; // dec R7

        // Write value in given register to SP
        this.ram.write(this.reg[7], this.reg[regA]);

        this.reg.PC += 2;
    }

    /**
     * POP
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

        this.reg[7]--;
        this.ram.write(this.reg[7], this.reg.PC + 2);

        this.reg.PC = this.reg[regA];
    }

    /**
     * RET
     */
    RET() {
        this.reg.PC = this.ram.read(this.reg[7]);
        this.reg[7]++;
    }

    /**
     * JMP
     */
    JMP() {
        const regA = this.ram.read(this.reg.PC + 1);

        this.reg.PC = this.reg[regA];
    }

    /**
     * ADD R R
     */
    ADD() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);

        this.alu('ADD', regA, regB);

        this.reg.PC += 3;
    }

    /**
     * ST R R
     */
    ST() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.read.PC + 2);

        this.ram.write(this.reg[regA], this.reg[regB]);

        this.reg.PC += 3;
    }

    /**
     * IRET 
     */
    IRET() {
        for (let i = 7; i >= 0; i--) {
            this.reg[i] = this.ram.read(this.reg[7]);
            this.reg[7]++;
        }

        this.reg.PC = this.ram.read(this.reg[7]);
        this.reg[7]++;

        this.flags.interruptsEnabled = true;
    }

    /**
     * PRINT alpha
     */
    PRA() {
        const regA = this.ram.read(this.reg.PC + 1);
        console.log(String.fromCharCode(this.reg[regA]));
        this.reg.PC += 2;
    }
}

module.exports = CPU;
