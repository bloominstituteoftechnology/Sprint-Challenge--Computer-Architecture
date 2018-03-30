/**
 * LS-8 v2.0 emulator skeleton code
 */

const fs = require('fs');

// Instructions

const HLT = 0b00000001; // Halt CPU
// !!! IMPLEMENT ME
const LDI = 0b10011001;
const MUL = 0b10101010;
const PRN = 0b01000011;
const ADD = 0b10101000;
const PUSH = 0b01001101;
const POP = 0b01001100;
const CALL = 0b00001111;
const RET = 0b00010000;
const CMP = 0b10100000;
const JMP = 0b01010000;
const JEQ = 0b01010001;
const JNE = 0b01010010;

const SP = 7;

// Flag values for FL register
const FLAG_EQ = 1;
const FLAG_GT = 2;
const FLAG_LT = 4;
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

    // Special-purpose registers
    this.reg.PC = 0; // Program Counter
    this.reg.IR = 0; // Instruction Register
    this.reg.FL = 0;

    // Init the stack pointer
    this.reg[SP] = 0xf3;

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
    bt[ADD] = this.ADD;
    bt[PUSH] = this.PUSH;
    bt[POP] = this.POP;
    bt[CALL] = this.call;
    bt[RET] = this.return;
    bt[CMP] = this.CMP;
    bt[JMP] = this.JMP;
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
    }, 1);
  }

  /**
   * Stops the clock
   */
  stopClock() {
    clearInterval(this.clock);
  }

  setFlag(flag, value) {
    if (value === true) {
      //set flag to 1
      this.reg.FL = this.reg.FL | flag;
    } else {
      // Set flag to 0
      this.reg.FL = this.reg.FL & ~flag;
    }
  }

  getFlag(flag) {
    return (this.reg.FL & flag) === flag;
  }

  /**
   * ALU functionality
   *
   * op can be: ADD SUB MUL DIV INC DEC CMP
   */
  alu(op, regA, regB) {
    switch (op) {
      case 'AND':
        this.reg[regA] = this.reg[regA] & this.reg[regB];
        break;

      case 'MUL':
        // !!! IMPLEMENT ME
        this.reg[regA] = this.reg[regA] * this.reg[regB];
        break;

      case 'ADD':
        this.reg[regA] = this.reg[regA] + this.reg[regB];
        break;

      case 'CMP':
        this.setFlag(FLAG_EQ, this.reg[regA] === this.reg[regB]);
        this.setFlag(FLAG_GT, this.reg[regA] > this.reg[regB]);
        this.setFlag(FLAG_LT, this.reg[regA] < this.reg[regB]);
        break;
    }
  }

  /**
   * Advances the CPU one cycle
   */
  tick() {
    // Load the instruction register (IR) from the current PC
    // !!! IMPLEMENT ME
    this.reg.IR = this.ram.read(this.reg.PC);

    // Debugging output
    // console.log(`${this.reg.PC}: ${this.reg.IR.toString(2)}`);

    // Based on the value in the Instruction Register, locate the
    // appropriate hander in the branchTable
    // !!! IMPLEMENT ME
    const handler = this.branchTable[this.reg.IR];

    // Check that the handler is defined, halt if not (invalid
    // instruction)
    // !!! IMPLEMENT ME
    if (handler === undefined) {
      console.log(`ERROR: Unknown Instruction ${this.reg.IR.toString(2)}`);
      console.log('Instruction undefined: ', handler);
      this.stopClock();
      return;
    }

    let operandA = this.ram.read(this.reg.PC + 1);
    let operandB = this.ram.read(this.reg.PC + 2);

    // We need to use call() so we can set the "this" value inside
    // the handler (otherwise it will be undefined in the handler)
    const newPC = handler.call(this, operandA, operandB);

    let bitMask = (this.reg.IR >> 6) & 0b00000011;

    // Increment the PC register to go to the next instruction
    // !!! IMPLEMENT ME
    if (newPC === undefined) {
      this.reg.PC = this.reg.PC + bitMask + 1;
    } else {
      this.reg.PC = newPC;
    }
  }

  // INSTRUCTION HANDLER CODE:

  ADD(regA, regB) {
    this.alu('ADD', regA, regB);
  }

  AND(regA, regB) {
    this.alu('AND', regA, regB);
  }

  // Call function
  CALL() {
    this.reg[SP]--;
    this.ram.write(this.reg[SP], this.reg.PC + 2);

    const regA = this.ram.read(this.reg.PC + 1);
    this.reg.PC = this.reg[regA];
  }

  // Compare function
  CMP(regA, regB) {
    this.alu('CMP', regA, regB);
  }

  /**
   * HLT
   */
  HLT() {
    // !!! IMPLEMENT ME
    this.stopClock();
  }

  // JMP if Equal
  JEQ(reg) {
    if (this.getFlag(FLAG_EQ)) {
      return this.reg[reg];
    }
  }

  // JMP function
  JMP(reg) {
    return this.reg[reg];
  }

  JNE(reg) {
    if (!this.getFlag(FLAG_EQ)) {
      return this.reg[reg];
    }
  }

  /**
   * LDI R,I
   */
  LDI(reg, value) {
    // !!! IMPLEMENT ME
    this.reg[reg] = value;
  }

  /**
   * MUL R,R
   */
  MUL(regA, regB) {
    // !!! IMPLEMENT ME
    this.alu('MUL', regA, regB);
  }

  /**
   * PRN R
   */
  PRN(reg) {
    // !!! IMPLEMENT ME
    console.log(this.reg[reg]);
  }

  PUSH(regNum) {
    let value = this.reg[regNum];
    this.reg[SP] = this.reg[SP] - 1;

    this.ram.write(this.reg[SP], value);
  }

  RET() {
    this.reg.PC = this.ram.read(this.reg[SP]);
    this.reg[SP]++;
  }

  // pushHelper(value) {
  //   this.reg[SP]--;
  //   this.ram.write(this.reg[SP], value);
  // }

  //   popHelper()
}

module.exports = CPU;
