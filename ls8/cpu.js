/**
 * LS-8 v2.0 emulator skeleton code
 */

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

    this.branchTable = new Array();

    this.branchTable[153] = (a, b) => this.handle_LDI(a, b);
    this.branchTable[67] = a => this.handle_PRN(a);
    this.branchTable[170] = (a, b) => this.handle_MUL(a, b);
    this.branchTable[72] = a => this.handle_CALL(a);
    this.branchTable[168] = (a, b) => this.handle_ADD(a, b);
    this.branchTable[77] = a => this.handle_PUSH(a);
    this.branchTable[76] = a => this.handle_POP(a);
    this.branchTable[160] = (a, b) => this.handle_CMP(a, b);
    this.branchTable[81] = a => this.handle_JEQ(a);
    this.branchTable[82] = a => this.handle_JNE(a);
    this.branchTable[80] = a => this.handle_JMP(a);
    this.branchTable[9] = () => this.handle_RET();
    this.branchTable[1] = () => this.handle_HLT();

    // Special-purpose registers
    this.PC = 0; // Program Counter
    this.SP = 7;
    this.FL = null;
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
      case "ADD":
        return regA + regB;
      case "SUB":
        return regA - regB;
      case "MUL":
        return regA * regB;
      case "DIV":
        return regA / regB;
    }
  }

  handle_LDI(operandA, operandB) {
    console.log("In reg " + operandA + " write " + operandB);
    this.reg[operandA] = operandB;
  }

  handle_PRN(operandA) {
    console.log(`Print of reg ${operandA} is ${this.reg[operandA]}`);
  }

  handle_MUL(operandA, operandB) {
    console.log(`Multiply ${this.reg[operandA]} by ${this.reg[operandB]}`);
    this.reg[operandA] = this.alu(
      "MUL",
      this.reg[operandA],
      this.reg[operandB]
    );
    console.log(`Reg ${operandA} is now equal to ${this.reg[operandA]}`);
  }

  handle_CALL(operandA) {
    // this.stopClock();
    console.log(`Reg ${operandA} is called (${this.reg[operandA]})`);
    this.subRoutine(this.reg[operandA]);
  }

  handle_ADD(operandA, operandB) {
    console.log(`Add ${this.reg[operandA]} to ${this.reg[operandA]}`);
    this.reg[operandA] = this.alu(
      "ADD",
      this.reg[operandA],
      this.reg[operandB]
    );
    console.log(`Reg ${operandA} is now equal to ${this.reg[operandA]}`);
  }

  handle_PUSH(operandA) {
    if (this.reg[this.SP] === 0) this.reg[this.SP] = 0xf4;
    this.reg[this.SP]--;
    console.log(`PUSH ${this.reg[operandA]} to ram ${this.reg[this.SP]}`);
    this.ram.write(this.reg[this.SP], this.reg[operandA]);
  }

  handle_POP(operandA) {
    console.log(`POP ram ${this.reg[this.SP]} to reg ${operandA}`);
    this.reg[operandA] = this.ram.read(this.reg[this.SP]);
    this.reg[this.SP]++;
  }

  handle_CMP(operandA, operandB) {
    console.log("CMP");
    if (this.reg[operandA] === this.reg[operandB]) {
      this.FL = "equal";
    }
  }

  handle_JEQ(operandA) {
    console.log("JEQ");
    if (this.FL === "equal") {
      this.handle_JMP(operandA);
      this.FL = null;
    }
  }

  handle_JNE(operandA) {
    console.log("JNE");
    if (!this.FL) {
      this.handle_JMP(operandA);
    }
  }

  handle_JMP(operandA) {
    console.log("JMP");
    console.log("operandA", operandA);
    // this.PC = operandA;
  }

  handle_RET() {
    console.log("RET");
    // this.startClock();
  }

  handle_HLT() {
    console.log("HLT");
    this.stopClock();
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
    const IR = this.ram.read(this.PC);

    // Debugging output
    // const IRstring = IR.toString(2);
    // console.log(`${this.PC}: ${IR.toString(2)}`);
    // console.log(`${this.PC}: ${IR}`);

    // Get the two bytes in memory _after_ the PC in case the instruction
    // needs them.

    // !!! IMPLEMENT ME
    const operandA = this.ram.read(this.PC + 1);
    const operandB = this.ram.read(this.PC + 2);

    // Execute the instruction. Perform the actions for the instruction as
    // outlined in the LS-8 spec.
    // const LDI = 153,
    //   PRN = 67,
    //   MUL = 170,
    //   CALL = 72,
    //   ADD = 24,
    //   HLT = 1;

    this.branchTable[IR](operandA, operandB);

    // Increment the PC register to go to the next instruction. Instructions
    // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
    // instruction byte tells you how many bytes follow the instruction byte
    // for any particular instruction.
    const instLen = (IR >> 6) + 1;
    this.PC += instLen;
  }

  subRoutine(call) {
    const IR = this.ram.read(call);
    console.log(`IR is ${IR}`);
    if (IR === 9) {
      return this.branchTable[IR]();
    }
    const operandA = this.ram.read(call + 1);
    const operandB = this.ram.read(call + 2);
    this.branchTable[IR](operandA, operandB);
    const instLen = (IR >> 6) + 1;
    call += instLen;
    this.subRoutine(call);
  }
}

module.exports = CPU;
