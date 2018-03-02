// import { setInterval } from 'timers';

/**
 * LS-8 v2.0 emulator skeleton code
 */

const fs = require('fs');

// Special Registers
const IM = 5; // R5 = Interrupt Mask (IM)
const IS = 6; // R6 = Interrupt Status (IS)
const SP = 7; // R7 = Stack Pointer (SP)

// Instructions
const HLT = 0b00011011; // Halt CPU
const ADD = 0b00001100;
const CALL = 0b00001111;
const IRET = 0b00011010; // Interrupt return
const JMP = 0b00010001;
const LDI = 0b00000100;
const MUL = 0b00000101;
const NOP = 0b00000000;
const PRA = 0b00000111; // print alpha char value
const PRN = 0b00000110; // print numeric value
const POP = 0b00001011;
const PUSH = 0b00001010;
const RET = 0b00010000;
const ST = 0b00001001;
const CMP = 0b00010110; // Compare instruction
const JEQ = 0b00010011; // Jump if Equal
const JNE = 0b00010100; // Jump if NOT Equal

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

        this.reg[SP] = 0xf8;

        // Special-purpose registers
        this.reg.PC = 0; // Program Counter
        this.reg.IR = 0; // Instruction Register

        this.flags = {
            interruptsEnabled: true,
            equal: null,
        };

        this.setupBranchTable();
    }

	/**
	 * Sets up the branch table
	 */
    setupBranchTable() {
        let bt = {};

        bt[HLT] = this.HLT; // Halt and exit
        bt[LDI] = this.LDI; // Load Immediate
        bt[MUL] = this.MUL;
        bt[PRN] = this.PRN; // Print
        bt[ADD] = this.ADD;
        bt[CALL] = this.CALL;
        bt[POP] = this.POP;
        bt[PUSH] = this.PUSH;
        bt[RET] = this.RET;
        bt[JMP] = this.JMP;
        bt[ST] = this.ST; // STore val in regB in addr stored in regA
        bt[PRA] = this.PRA; // print alpha char stored in given reg
        bt[IRET] = this.IRET; // Interrupt Return
        bt[NOP] = this.NOP;
        bt[CMP] = this.CMP; // Compare instr.
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

        // console.log("RAM dump");
        // for (let i = 0; i < 15; i++) {
        //     console.log(this.ram.read(i).toString(2));
        // }

        const _this = this;

        this.clock = setInterval(() => {
            _this.tick();
        }, 1);

        this.timerHandle = setInterval(() => {
            // Trigger timer interrupt
            // Need IM and IS registers
            // e.g.-set bit 0 of IS to 1
            // this.reg[IS] = this.reg[IS] | 0b00000001;
            this.reg[IS] |= 0b00000001; // shorthand operator

        }, 1000); // 1sec
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
        // console.log(`valA: ${valA}  valB: ${valB}`);

        switch (op) {
            case 'MUL':
                // !!! IMPLEMENT ME
                this.reg[regA] = (valA * valB) & 255;
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
        // Check IS (R6) if an interrupt happened
        // if it did, jump to that interrupt handler
        const maskedInterrupts = this.reg[IS] & this.reg[IM];
        if (this.flags.interruptsEnabled && maskedInterrupts) {
            for (let i = 0; i < 8; i++) {
                if (((maskedInterrupts >> i) & 1) == 1) {
                    // Handling interrupt
                    this.flags.interruptsEnabled = false;

                    // console.log("this.reg[IS] before: " + parseInt(this.reg[IS], 2));
                    // Clear ith bit in IS reg
                    this.reg[IS] = this.reg[IS] & ~(1 << i);

                    // Push PC on stack
                    this.reg[SP]--; // dec R7 (SP)
                    this.ram.write(this.reg[SP], this.reg.PC);

                    // Push remaining registers on the stack
                    for (let j = 0; j < 8; j++) {
                        this.reg[SP]--;
                        this.ram.write(this.reg[SP], this.reg[j]);
                    }

                    // Look up the handler address in the interrupt vector table
                    const vectorTableEntry = 0xf8 + i;
                    const handlerAddress = this.ram.read(vectorTableEntry);

                    // Set PC to handler
                    this.reg.PC = handlerAddress;

                    // console.log("handling interrupt! " + this.reg[IS] + " " + i);
                    break;
                }
            }
            // this.reg[IS] = 0;
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
     * ADD
     */
    ADD() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);

        this.alu('ADD', regA, regB);

        this.reg.PC += 3; // move PC
    }

    /**
     * CALL R
     */
    CALL() {
        const regA = this.ram.read(this.reg.PC + 1);

        // Push address of next instruction on stack
        this.reg[SP]--; // dec R7 (SP reg)
        this.ram.write(this.reg[SP], this.reg.PC + 2);

        // Jump to the address stored in regA
        this.reg.PC = this.reg[regA];
    }

    /**
     * CMP R R
     */
    CMP() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);

        // regA AND inverseOf(regB)
        if ((this.reg[regA] & ~this.reg[regB]) === 0) {
            this.flags.equal = true;
        } else this.flags.equal = false;

        // console.log("Compare result: " + this.flags.equal);

        this.reg.PC += 3;
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
    LDI() {
        const regA = this.ram.read(this.reg.PC + 1);
        const val = this.ram.read(this.reg.PC + 2); // immed. val.

        this.reg[regA] = val;
        this.reg.PC += 3; // move PC
    }

    /**
     * MUL R,R
     */
    MUL() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2);

        this.alu('MUL', regA, regB);

        this.reg.PC += 3; // move PC
    }

    /**
     * PRA R
     */
    PRA() {
        const regA = this.ram.read(this.reg.PC + 1);
        console.log(String.fromCharCode(this.reg[regA]));

        this.reg.PC += 2;
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
     * POP R
     */
    POP() {
        const regA = this.ram.read(this.reg.PC + 1);
        const stackVal = this.ram.read(this.reg[SP]);

        this.reg[regA] = stackVal;

        this.reg[SP]++;
        this.reg.PC += 2;
    }

    /**
     * PUSH R
     */
    PUSH() {
        const regA = this.ram.read(this.reg.PC + 1);

        this.reg[SP]--; // dec R7 (SP reg)
        this.ram.write(this.reg[SP], this.reg[regA]);

        this.reg.PC += 2;
    }

    /**
     * IRET
     */
    IRET() {
        // Pop registers off the stack
        for (let j = 7; j >= 0; j--) {
            this.reg[j] = this.ram.read(this.reg[SP]);
            this.reg[SP]++;
        }

        // Pop PC off stack
        this.reg.PC = this.ram.read(this.reg[SP]);
        this.reg[SP]++;

        // Re-enable interrupts
        this.flags.interruptsEnabled = true;
    }

    /**
     * RET
     */
    RET() {
        this.reg.PC = this.ram.read(this.reg[SP]);

        this.reg[SP]++; // incr SP reg
    }

    /**
     * ST R R
     */
    ST() {
        const regA = this.ram.read(this.reg.PC + 1);
        const regB = this.ram.read(this.reg.PC + 2); // val in regB

        this.ram.write(this.reg[regA], this.reg[regB]);
        this.reg.PC += 3; // move PC
    }

    /**
     * JMP R
     */
    JMP() {
        const regA = this.ram.read(this.reg.PC + 1);

        this.reg.PC = this.reg[regA];
        // console.log("regA: " + this.reg[regA]);
    }

    /**
     * JEQ R -- Jump if Equal (flag is set)
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
     * JNE R -- Jump if NOT Equal (flag is not set)
     */
    JNE() {
        if (this.flags.equal === false) {
            const regA = this.ram.read(this.reg.PC + 1);
            this.reg.PC = this.reg[regA];
            //console.log("In JNE!");
        } else {
            this.reg.PC += 2;
        }
    }
}

module.exports = CPU;
