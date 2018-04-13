/**
 * LS-8 v2.0 emulator skeleton code
 */

const ADD = 0b10101000;
//const AND = 10110011;
const CALL = 0b01001000;
//const CMP =
//const DEC =
//const DIV =
const HLT = 0b00000001;
//const INC =
// const INT =
//const IRET =
//const JEQ = 
// const JGT =
//const JLT =
//const JMP =
//const JNE =
//const LD =
const LDI = 0b10011001;
//const MOD =
const MUL = 0b10101010;
//const NOP =
//const NOT =
//const OR =
const POP = 0b01001100;
//const PRA =
const PRN = 0b01000011;
const PUSH = 0b01001101;
const RET = 0b00001001;
const ST = 0b10011010;
//const SUB = 10101001;
//const XOR = 10110010;

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

        this.reg = new Array(8).fill(0); // General-purpose registers R0-R7
        
        this.reg.PC = 0; // Program Counter
				this.reg[SP] = 0xF4;
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
        this.clock = setInterval(() => {
            this.tick();
        }, 1); // 1 ms delay == 1 KHz clock == 0.000001 GHz
				//this.clock = setInterval(() => {
				//	this.interruptTimer();
				//}, 1000);
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
     * The ALU is responsible for math and comparisons.
     *
     * If you have an instruction that does math, i.e. MUL, the CPU would hand
     * it off to it's internal ALU component to do the actual work.
     *
     * op can be: ADD SUB MUL DIV INC DEC CMP
     */
    alu(op, regA, regB) {
        switch (op) {
          case MUL:
            this.reg[regA] = this.reg[regA] * this.reg[regB];
            break;
					case ADD:
						this.reg[regA] = this.reg[regA] + this.reg[regB];
						break;
        }
    }

    /**
     * Advances the CPU one cycle
     */
    tick() {
        let IR = this.ram.read(this.reg.PC);
        let operandA = this.ram.read(this.reg.PC + 1);
        let operandB = this.ram.read(this.reg.PC + 2); 
				let advancePC = true;

        switch(IR) {
					case CALL:
						advancePC = false;
						this.ram.write(this.reg[SP], this.reg.PC + 2);
						this.reg.PC = this.reg[operandA];
						break; 
          case HLT:
            this.stopClock();
            break; 
          case LDI:
            this.reg[operandA] = operandB;
            break;
          case MUL:
					case ADD:
            this.alu(IR, operandA, operandB);
            break; 
          case POP:
            this.reg[operandA] = this.ram.read(this.reg[SP]);
            this.reg[SP] += 1;
            break; 
          case PRN:
            console.log(this.reg[operandA]);
            break;
					case PUSH:
            this.reg[SP] -= 1;
            this.ram.write(this.reg[SP], this.reg[operandA]);
            break; 
					case RET:
						advancePC = false;
						this.reg.PC = this.ram.read(this.reg[SP]);
						break;
					case ST:
						this.reg[this.reg[operandA]] = this.reg[operandB];
						break;
        }	
			if (advancePC) {
        let operandCount = (IR >>> 6) & 0b11;
        let totalInstructionLen = operandCount + 1;
        this.reg.PC += totalInstructionLen;
			}
    }
//	interruptTimer() {
//		
//	}
}

module.exports = CPU;
