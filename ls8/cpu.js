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
const CMP = 0b10100000;
const JEQ = 0b01010001;
const JNE = 0b01010010;
const JMP = 0b01010000;

const index = 7;
const RAM = require('./ram.js');
class CPU {

    /**
     * Initialize the CPU
     */
    constructor(ram) {
        this.ram = ram;

        this.reg = new Array(8).fill(0); // General-purpose registers R0-R7

        // Special-purpose registers
        this.PC = 0; // Program Counter
        this.SP = this.reg[index] = 0xF4;
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
          case ADD:
              this.reg[regA] = this.reg[regA] + this.reg[regB];
              break;
          case 'SUB':
              return this.reg[regA] - this.reg[regB];
              break;
          case MUL:
              this.reg[regA] = this.reg[regA] * this.reg[regB];
              // this.PC += 3;
              break;
          case 'DIV':
              return this.reg[regA] / this.reg[regB];
              break;
          case 'INC':
              return regA++;
              break;
          case 'DEC':
              return regA--;
              break;
          case CMP:
              if (this.reg[regA] === this.reg[regB]) this.reg.fl = '00000001';
              else if (this.reg[regA] > this.reg[regB]) this.reg.fl = '00000010';
              else this.reg.fl = '00000100';
              break;
          default:
              console.log("Unknown instruction: " + IR.toString(2));
              this.stopClock();
              return;
        }
    }
    pushValue(v) {
      this.SP--;
      this.ram.write(this.SP, v);
    }
    /**
     * Advances the CPU one cycle
     */
    tick() {
        // Load the instruction register (IR--can just be a local variable here)
        // from the memory address pointed to by the PC. (I.e. the PC holds the
        // index into memory of the instruction that's about to be executed
        // right now.)
        let IR = this.ram.read(this.PC);
        // !!! IMPLEMENT ME

        // Debugging output
        // console.log(`${this.PC}: ${IR.toString(2)}`);

        // Get the two bytes in memory _after_ the PC in case the instruction
        // needs them.

        // !!! IMPLEMENT ME
        const opA = this.ram.read(this.PC + 1);
        const opB = this.ram.read(this.PC + 2);

        // Execute the instruction. Perform the actions for the instruction as
        // outlined in the LS-8 spec.

        // !!! IMPLEMENT ME

        this.pcAdvance = true;
        switch(IR) {
            case LDI:
                // Set the value in a register
                this.reg[opA] = opB; // Next instruction
                break;
            case PRN:
                console.log(this.reg[opA]);
                break;
            case HLT:
                this.stopClock();
                break;
            case PUSH:
                this.SP--;
                this.ram.write(this.reg[index], this.reg[opA]);
                break;
            case POP:
                this.reg[opA] = this.ram.read(this.reg[index]);
                this.SP++;
                break;
            case CALL:
                this.pushValue(this.PC + 2);
                this.PC = this.reg[opA];
                this.pcAdvance = false;
                break;
            case RET:
                this.PC = this.ram.read(this.SP);
                this.SP++;
                this.pcAdvance = false;
                break;
            case JEQ:
                if(this.reg.fl === '00000001') {
                  this.PC  = this.reg[opA];
                  this.pcAdvance = false;
                }
                break;
            case JNE:
                if(this.reg.fl !== '00000001') {
                  this.PC = this.reg[opA];
                  this.pcAdvance = false;
                }
                break;
            case JMP:
                this.PC = this.reg[opA];
                this.pcAdvance = false;
                break;
            default:
                this.alu(IR, opA, opB);
                break;
        }
        // Increment the PC register to go to the next instruction. Instructions
        // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
        // instruction byte tells you how many bytes follow the instruction byte
        // for any particular instruction.

        // !!! IMPLEMENT ME
        if (this.pcAdvance) {
          this.PC += (IR >> 6) + 1;
        }
    }
}
module.exports = CPU;
