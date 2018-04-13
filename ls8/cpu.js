/**
 * LS-8 v2.0 emulator skeleton code
 */

/**
 * Class for simulating a simple Computer (CPU & memory)
 */

const LDI = 0b10011001;
const PRN = 0b01000011;
const HLT = 0b00000001;
const MUL = 0b10101010;
const PUSH = 0b01001101;
const POP = 0b01001100;
const CALL = 0b01001000;
const RET = 0b00001001;
const ADD = 0b10101000;
const ST = 0b10011010;
const JMP = 0b01010000;
const PRA = 0b01000010;
const IRET = 0b00001011;

const IM = 0x05;  // Interrupt mask register R5
const IS = 0x06;  // Interrupt status register R6
const SP = 0x07;  // Stack Pointer

// Interrupt mask bits
const intMask = [
    (0x1 << 0), // timer
    (0x1 << 1), // keyboard
    (0x1 << 2), // reserved
    (0x1 << 3), // reserved
    (0x1 << 4), // reserved
    (0x1 << 5), // reserved
    (0x1 << 6), // reserved
    (0x1 << 7), // reserved
];

class CPU {

    /**
     * Initialize the CPU
     */
    constructor(ram) {
        this.ram = ram;

        this.reg = new Array(8).fill(0); // General-purpose registers R0-R7

        
        // Special-purpose registers
        this.reg.PC = 0; // Program Counter
        this.reg.FL = 0; // Flags

        this.interruptsEnabled = true;

        this.reg[SP] = 0xf4;    // Stack empty
        this.reg[IM] = 0;       // All interrupts masked
        this.reg[IS] = 0;       // No interrupts active
        this.setupBranchTable();
    }

    setupBranchTable() {
        let bt = {}

        bt[ST] = this.handle_ST;
        bt[ADD] = this.handle_ADD;
        bt[HLT] = this.handle_HLT;
        bt[JMP] = this.handle_JMP;
        bt[LDI] = this.handle_LDI;
        bt[MUL] = this.handle_MUL;
        bt[POP] = this.handle_POP;
        bt[PRA] = this.handle_PRA;
        bt[PRN] = this.handle_PRN;
        bt[RET] = this.handle_RET;
        bt[CALL] = this.handle_CALL;
        bt[IRET] = this.handle_IRET;
        bt[PUSH] = this.handle_PUSH;

        this.branchTable = bt;
    }
	
    /**
     * Store value in memory address, useful for program loading
     */
    poke(address, value) {
        this.ram.write(address, value);
    }

    /**
   * Raise an interrupt
   * 
   * @param n Interrupt number, 0-7
   */
    raiseInterrupt(n) {
        this.reg[IS] |= intMask[n];
    }

    /**
     * Starts the clock ticking on the CPU
     */
    startClock() {
        this.clock = setInterval(() => {
            this.tick();
        }, 1); // 1 ms delay == 1 KHz clock == 0.000001 GHz

        this.timerInterrupt = setInterval(() => {
            // Set the timer bit in the IS register
            this.raiseInterrupt(0);
        }, 1000);
    }

    /**
     * Stops the clock
     */
    stopClock() {
        clearInterval(this.clock);
        clearInterval(this.timerInterrupt);
    }

    /**
     * ALU functionality
     *
     * The ALU is responsible for math and comparisons.
     *
     * If you have an instruction that does math, i.e. MUL, the CPU would hand
     * it off to it's internal ALU component to do the actual work.
     *
     * op can be: ADD SUB MUL DIV INC DEC CMP
     */
    alu(op, regA, regB) {
        switch (op) {
            case 'MUL':
                this.reg[regA] = this.reg[regA] * this.reg[regB];
                // console.log('regA log:', regA);
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

        if (this.interruptsEnabled) {
            // Take the current interrupts and mask them out with the interrupt
            // mask
            const maskedInterrupts = this.reg[IS] & this.reg[IM];
      
            // Check all the masked interrupts to see if they're active
            for (let i = 0; i < 8; i++) {
              
              // If it's still 1 after being masked, handle it
              if (((maskedInterrupts >> i) & 0x01) === 1) {
      
                // Only handle one interrupt at a time
                this.interruptsEnabled = false;
      
                // Clear this interrupt in the status register
                this.reg[IS] &= ~intMask[i];
      
                // Push return address
                this.reg[SP] = this.reg[SP] - 1;
                this.ram.write(this.reg[SP], this.reg.PC);
                // this._push(this.reg.PC);
      
                // Push flags
                this.reg[SP] = this.reg[SP] - 1;
                this.ram.write(this.reg[SP], this.reg.FL);
                // this._push(this.reg.FL);
      
                // Push registers R0-R6
                for (let r = 0; r <= 6; r++) {
                    this.reg[SP] = this.reg[SP] - 1;
                    this.ram.write(this.reg[SP], this.reg[r]);
                //   this._push(this.reg[r]);
                }
      
                // Look up the vector (handler address) in the
                // interrupt vector table
                const vector = this.ram.read(0xf8 + i);
      
                this.reg.PC = vector; // Jump to it
      
                // Stop looking for more interrupts, since we do one
                // at a time
                break;
              }
            }
          }

        // Load the instruction register (IR--can just be a local variable here)
        // from the memory address pointed to by the PC. (I.e. the PC holds the
        // index into memory of the instruction that's about to be executed
        // right now.)
        
        let IR = this.ram.read(this.reg.PC);

        // Debugging output
        // console.log(`${this.reg.PC}: ${IR.toString(2)}`);
        // console.log(typeof IR, IR);

        // Get the two bytes in memory _after_ the PC in case the instruction
        // needs them.

        const operandA = this.ram.read(this.reg.PC + 1);
        const operandB = this.ram.read(this.reg.PC + 2);

        // Execute the instruction. Perform the actions for the instruction as
        // outlined in the LS-8 spec.

        const handlerReturn = this.branchTable[IR](this, operandA, operandB);

        // Increment the PC register to go to the next instruction. Instructions
        // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
        // instruction byte tells you how many bytes follow the instruction byte
        // for any particular instruction.

        if(handlerReturn === undefined) {
            const inc = (IR >>> 6) + 1;
            this.reg.PC += inc;
        } else {
            this.reg.PC = handlerReturn;
        }

    }

    handle_LDI (cpu, operandA, operandB) {
        cpu.reg[operandA] = operandB;
    }

    handle_PRN (cpu, operandA) {
        console.log(cpu.reg[operandA]);
    }

    handle_HLT (cpu) {
        cpu.stopClock();
    }

    handle_MUL (cpu, operandA, operandB) {
        cpu.alu('MUL', operandA, operandB);
    }
   
    handle_ADD (cpu, operandA, operandB) {
        cpu.alu('ADD', operandA, operandB);
    }

    handle_PUSH (cpu, opA) {
        // console.log('>>>>>>>>>>>>>>>>>', opA)
        cpu.reg[SP] = cpu.reg[SP] - 1;
        cpu.ram.write(cpu.reg[SP], cpu.reg[opA]);
    }
    
    handle_POP (cpu, opA) {
        cpu.reg[opA] = cpu.ram.read(cpu.reg[SP]);
        cpu.reg[SP]++;
    }

    handle_CALL (cpu, opA) {
        // handle_PUSH(cpu.reg.PC + 2);
        cpu.reg[SP] = cpu.reg[SP] - 1;
        cpu.ram.write(cpu.reg[SP], cpu.reg.PC + 2);
        return cpu.reg[opA];
    }
    
    handle_RET (cpu) {
        const value = cpu.ram.read(cpu.reg[SP]);
        cpu.reg[SP]++;
        return value;
    }

    handle_ST(cpu, registerA, registerB) {
        cpu.ram.write(cpu.reg[registerA], cpu.reg[registerB]);
    }

    handle_JMP(cpu, givenRegister) {
        return cpu.reg[givenRegister];
    }

    handle_PRA(cpu, givenRegister) {
        console.log(String.fromCharCode(cpu.reg[givenRegister]));
    }

    handle_IRET(cpu) {
        for(let r = 6; r >= 0; r--) {
            cpu.reg[r] = cpu.ram.read(cpu.reg[SP]);
            cpu.reg[SP]++;
        }
        
        cpu.reg.FL = cpu.ram.read(cpu.reg[SP]);
        cpu.reg[SP]++;

        // cpu.pcAdvance = true;
        cpu.interruptsEnabled = true;

        return cpu.ram.read(cpu.reg[SP]++);
    }

}

module.exports = CPU;
