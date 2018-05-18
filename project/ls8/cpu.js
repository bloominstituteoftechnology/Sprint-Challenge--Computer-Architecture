const HLT = 0b00000001;
const LDI = 0b10011001;
const PRN = 0b01000011;
const ADD = 0b10101000;
const SUB = 0b10101001;
const MUL = 0b10101010;
const DIV = 0b10101011;
const INC = 0b01111000;
const DEC = 0b01111001;
const CMP = 0b10100000;
const JMP = 0b01010000;
const JNE = 0b01010010;
const JEQ = 0b01010001;
const PUSH = 0b01001101;
const POP = 0b01001100;

class CPU {
  constructor(ram) {
    this.ram = ram;
    this.reg = new Array(8).fill(0); // General-purpose registers R0-R7
    this.PC = 0; // Special-purpose register Program Counter
    this.handler = [];
    this.handler[LDI] = this.handle_LDI.bind(this);
    this.handler[PRN] = this.handle_PRN.bind(this);
    this.handler[HLT] = this.handle_HLT.bind(this);
    this.handler[MUL] = this.handle_MUL.bind(this);
    this.handler[PUSH] = this.handle_PUSH.bind(this);
    this.handler[POP] = this.handle_POP.bind(this);
  }

  poke(address, value) {
    this.ram.write(address, value);
  }

  startClock() {
    this.clock = setInterval(() => {
      this.tick();
    }, 1); // 1 ms delay == 1 KHz clock == 0.000001 GHz
  }

  stopClock() {
    clearInterval(this.clock);
  }

  alu(op, regA, regB) {
    switch (op) {
      case MUL:
        this.reg[regA] *= this.reg[regB];
        break;
      default:
        console.log("No operation for alu selected..");
        this.stopClock();
        break;
    }
  }

  tick() {
    const IR = this.ram.read(this.PC);

    //console.log(`${this.PC}: ${IR.toString(2)}`); // should display IR only

    const operandA = this.ram.read(this.PC + 1);
    const operandB = this.ram.read(this.PC + 2);
    const h = this.handler[IR];

    if (h === undefined) {
      console.log("h is not a function yet");
      this.stopClock();
      return;
    }

    h(operandA, operandB); // where the handler is actually called in tick

    this.PC += (IR >> 6) + 1;
  }

  /******************************************HANDLERS******************************/
  handle_LDI(operandA, operandB) {
    this.reg[operandA] = operandB;
  }

  handle_PRN(operandA, operandB) {
    console.log(this.reg[operandA]);
  }

  handle_HLT(operandA, operandB) {
    this.stopClock();
  }

  handle_MUL(operandA, operandB) {
    this.alu(MUL, operandA, operandB);
  }

  handle_PUSH() {}

  handle_POP() {}
}

module.exports = CPU;

/**
 * LS-8 v2.0 emulator skeleton code
 */

/**
 * Class for simulating a simple Computer (CPU & memory)
 */

/**
 * Initialize the CPU
 */

// Special-purpose registers

/**
 * Store value in memory address, useful for program loading
 */

/**
 * Starts the clock ticking on the CPU
 */

/**
 * Stops the clock
 */

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

/**
 * Advances the CPU one cycle
 */

// Load the instruction register (IR--can just be a local variable here)
// from the memory address pointed to by the PC. (I.e. the PC holds the
// index into memory of the instruction that's about to be executed
// right now.)

// !!! IMPLEMENT ME

// Debugging output
// console.log(`${this.PC}: ${IR.toString(2)}`);

// Get the two bytes in memory _after_ the PC in case the instruction
// needs them.

// !!! IMPLEMENT ME

// Execute the instruction. Perform the actions for the instruction as
// outlined in the LS-8 spec.

// !!! IMPLEMENT ME

// Increment the PC register to go to the next instruction. Instructions
// can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
// instruction byte tells you how many bytes follow the instruction byte
// for any particular instruction.

// !!! IMPLEMENT ME

// switch (IR) {
//   case HLT:
//     this.stopClock();
//     break;
//   case LDI:
//     this.reg[operandA] = operandB;
//     break;
//   case PRN:
//     console.log(this.reg[operandA]);
//     break;
//   case MUL:
//     this.alu(MULT, operandA, operandB);
//     break;
//   case PUSH:
//     this.reg.push();
//     break;
//   case POP:
//     this.reg.pop();
//     break;
//   default:
//     console.log(`\n\n***\nUnknown instruction at ${this.PC}\n***\n\n`);
//     this.stopClock();
// }
