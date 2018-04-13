/**
 * LS-8 v2.0 emulator skeleton code
 */
const LDI = 0b10011001;
const PRN = 0b01000011;
const HLT = 0b00000001;
const MUL = 0b10101010;
const ADD = 0b10101000;
const CMP = 0b10100000;
const PUSH = 0b01001101;
const POP = 0b01001100;
const CALL = 0b01001000;
const RET = 0b00001001;

const SP = 0b00000111;
const KEYPRESSEDADDRESS = 0xf4; //address above start of stack
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
    this.reg[SP] = KEYPRESSEDADDRESS;
    this.setupBranchTable();
    this.pcAdvance = true;
  }

  setupBranchTable() {
    let bt = {};

    bt[ADD] = this.handle_ADD;
    bt[HLT] = this.handle_HLT;
    bt[LDI] = this.handle_LDI;
    bt[MUL] = this.handle_MUL;
    bt[POP] = this.handle_POP;
    bt[PUSH] = this.handle_PUSH;
    bt[PRN] = this.handle_PRN;
    bt[CALL] = this.handle_CALL;
    bt[RET] = this.handle_RET;

    // Bind all the functions to this so we can call them later
    for (let k of Object.keys(bt)) {
      bt[k] = bt[k].bind(this);
    }

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
      case MUL:
        let multiplication = this.reg[regA] * this.reg[regB];
        this.reg[regA] = multiplication;
        break;
      case ADD:
        let addition = this.reg[regA] + this.reg[regB];
        this.reg[regA] = addition;
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
    let IR = this.ram.read(this.reg.PC);
    // !!! IMPLEMENT ME

    // Debugging output
    console.log(`${this.reg.PC}: ${IR.toString(2)}`);

    // Get the two bytes in memory _after_ the PC in case the instruction
    // needs them.

    let operandA = this.ram.read(this.reg.PC + 1);
    let operandB = this.ram.read(this.reg.PC + 2);

    this.advancePC = true;

    const handler = this.branchTable[IR];
    handler(operandA, operandB);

    // Execute the instruction. Perform the actions for the instruction as
    // outlined in the LS-8 spec.
    // !!! IMPLEMENT ME

    // Increment the PC register to go to the next instruction. Instructions
    // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
    // instruction byte tells you how many bytes follow the instruction byte
    // for any particular instruction.

    if (this.advancePC) {
      let operandCount = (IR >>> 6) & 0b11;
      this.reg.PC += operandCount + 1;
    }

    // !!! IMPLEMENT ME
  }

  handle_ADD(regA, regB) {
    this.alu(ADD, regA, regB);
  }

  handle_HLT() {
    this.stopClock();
  }

  handle_LDI(regA, regB) {
    this.reg[regA] = regB;
  }

  handle_MUL(regA, regB) {
    this.alu(MUL, regA, regB);
  }

  handle_POP(regA) {
    this.reg[regA] = this.ram.read(this.reg[SP]);
    this.reg[SP]++;
  }

  handle_PRN(regA) {
    console.log(this.reg[regA]);
  }

  handle_PUSH(regA) {
    this.reg[SP]--;
    this.ram.write(this.reg[SP], this.reg[regA]);
  }

  handle_CALL(regA) {
    this.reg.PC += 2;
    this.reg[SP] = regA;
    this.advancePC = false;
  }

  handle_RET() {
    this.reg.PC = this.ram.read(this.reg[SP]);
    this.advancePC = false;
  }
}

module.exports = CPU;
