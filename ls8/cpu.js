/* eslint-disable */
/**
 * LS-8 v2.0 emulator skeleton code
 */

const LDI = 0b10011001;
const PRN = 0b01000011;
const HLT = 0b00000001;
const ADD = 0b10110011;
const CALL = 0b01001000;
const CMP = 0b10100000;
const DEC = 0b01111001;
const DIV = 0b10101011;
const INC = 0b01111000;
const INT = 0b01001010;
const IRET = 0b00001011;
const JEQ = 0b01010001;
const JGT = 0b01010100;
const JLT = 0b01010011;
const JMP = 0b01010000;
const JNE = 0b01010010;
const LD = 0b10011000;
const MOD = 0b10101100;
const MUL = 0b10101010;
const NOP = 0b00000000;
const OR = 0b01110000;
const POP = 0b01001100;
const PRA = 0b01000010;
const PUSH = 0b01001101;
const RET = 0b00001001;
const ST = 0b10011010;
const SUB = 0b10101001;
const XOR = 0b10110010;

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

    // Special-purpose registers
    this.reg.PC = 0; // Program Counter
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
    let valA, valB;

    // load valA from regA
    valA = this.reg[regA];

    // load valB from regB
    valB = this.reg[regB];

    switch (op) {
      case ADD:
        this.reg[regA] = valA + valB;
        break;
      case SUB:
        this.reg[regA] = valA - valB;
        break;
      case MUL:
        this.reg[regA] = valA * valB;
        break;
      case DIV:
        this.reg[regA] = valA / valB;
        break;
      case INC:
        this.reg[regA] = valA + 1;
        break;
      case DEC:
        this.reg[regA] = valA - 1;
        break;
      case CMP:
        valA === valB;
        break;
    }
  }

  /**
   * Advances the CPU one cycle
   */
  tick() {
    // Load the instruction register (IR--can just be a local variable here)
    // from the memory address pointed to by the PC. (I.e. the PC holds the
    // index into memory of the instruction that's about to be executed
    // right now.)

    // !!! IMPLEMENT ME
    let IR = this.ram.read(this.reg.PC);

    // Debugging output
    console.log(`${this.reg.PC}: ${IR.toString(2)}`);

    // Get the two bytes in memory _after_ the PC in case the instruction
    // needs them.

    // !!! IMPLEMENT ME
    let operandA = this.ram.read(this.reg.PC + 1);
    let operandB = this.ram.read(this.reg.PC + 2);

    // Execute the instruction. Perform the actions for the instruction as
    // outlined in the LS-8 spec.

    // !!! IMPLEMENT ME
    switch (IR) {
      case LDI:
        this.reg[operandA] = operandB;
        break;
      case PRN:
        console.log(this.reg[operandA]);
        break;
      case HLT:
        this.stopClock();
        break;
      case ADD:
        this.alu(ADD, this.reg[operandA], this.reg[operandB]);
        break;
      case SUB:
        this.alu(SUB, this.reg[operandA], this.reg[operandB]);
        break;
      case MUL:
        this.alu(MUL, operandA, operandB);
        break;
      case DIV:
        this.alu(DIV, this.reg[operandA], this.reg[operandB]);
        break;
      case INC:
        this.alu(INC, this.reg[operandA], this.reg[operandB]);
        break;
      case DEC:
        this.alu(DEC, this.reg[operandA], this.reg[operandB]);
        break;
      case CMP:
        this.alu(CMP, this.reg[operandA], this.reg[operandB]);
        break;
      case PUSH:
        this.reg[SP]--;
        this.ram.write(this.reg[SP], this.reg[operandA]);
        break;
      case POP:
        this.reg[operandA] = this.ram.read(this.reg[SP]);
        this.reg[SP]++;
        break;
      case CALL:
        this.reg[SP]--;
        this.ram.write(this.reg[SP], this.reg[this.reg.PC + 2]);
        this.reg.PC = this.reg[operandA];
        break;
      case RET:
        this.reg.PC = this.ram.read(this.reg.PC + 2);
        this.reg[SP]++;
        break;
    }

    // Increment the PC register to go to the next instruction. Instructions
    // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
    // instruction byte tells you how many bytes follow the instruction byte
    // for any particular instruction.

    // !!! IMPLEMENT ME
    let toIncrement = IR >>> 6;
    toIncrement = parseInt(toIncrement, 10);
    toIncrement += 1;
    this.reg.PC += toIncrement;
  }
}

module.exports = CPU;
