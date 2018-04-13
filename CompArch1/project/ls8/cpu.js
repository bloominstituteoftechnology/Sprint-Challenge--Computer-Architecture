/**
 * LS-8 v2.0 emulator skeleton code
 */

// Instruction Sets
const ADD = 0b10101000; // ADD R R
const CALL = 0b01001000; // CALL R
const CMP = 0b10100000; // CMP R R
const HLT = 0b00000001; // Halt CPU
const LDI = 0b10011001; // LDI R,I(mmediate)
const MUL = 0b10101010; // MUL R,R
const POP = 0b01001100; // Pop R
const PRN = 0b01000011; // Print numeric register
const PUSH = 0b01001101; // Push R
const RET = 0b00001001; // Return
const JMP = 0b01010000; // Jump to address
const JEQ = 0b01010001; // If Flag set to `equal` Jump to address
const JNE = 0b01010010; // If `Equal` Flag is false `0` to Jump to address

// Flags
const L = 0b00000100; // Flag for Less-Than
const G = 0b00000010; // Flag for Greater-Than
const E = 0b00000001; // Flag for Equal

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
    switch (op) {
      case 'MUL':
        this.reg[0] = regA * regB;
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

    //&& assign instruction to IR from the current PC
    let IR = this.ram.read(this.reg.PC);

    // !!! IMPLEMENT ME

    // Debugging output
    //console.log(`${this.reg.PC}: ${IR.toString(2)}`);

    // Get the two bytes in memory _after_ the PC in case the instruction
    // needs them.

    //&& assign next two bytes of current PC to variables
    let operandA = this.ram.read(this.reg.PC + 1);
    let operandB = this.ram.read(this.reg.PC + 2);
    let addLen = true;

    // !!! IMPLEMENT ME

    // Execute the instruction. Perform the actions for the instruction as
    // outlined in the LS-8 spec.

    // console.log({
    //   PC: this.reg.PC,
    //   operandA,
    //   operandB,
    //   R0: this.reg[0],
    //   R1: this.reg[1],
    //   R2: this.reg[2]
    // });
    switch (IR) {
      // !!! IMPLEMENT ME
      // instruction for load and print
      case LDI:
        this.reg[operandA] = operandB;
        break;

      case CMP:
        if (this.reg[operandA] === this.reg[operandB]) this.reg.FL = E;
        if (this.reg[operandA] > this.reg[operandB]) this.reg.FL = G;
        if (this.reg[operandA] < this.reg[operandB]) this.reg.FL = L;
        break;

      case JMP:
        this.reg.PC = this.ram.read(this.reg.PC + 1);
        break;

      case JEQ:
        if (this.reg.FL === E) return (this.reg.PC = this.reg[operandA]);
        break;

      case JNE:
        if (this.reg.FL != E) return (this.reg.PC = this.reg[operandA]);
        break;

      // instruction for muliply
      // first load (LDI) assigns first value to reg[0]
      // second load (LDI) assigns second value to reg[1]
      case MUL:
        this.alu('MUL', this.reg[operandA], this.reg[operandB]);
        break;

      case PUSH:
        this.reg[7]--;
        this.ram.write(this.reg[7], this.reg[operandA]);
        break;

      case POP:
        this.reg[operandA] = this.ram.read(this.reg[7]);
        this.reg[7]++;
        break;

      case CALL:
        this.reg[7]--;
        this.ram.write(this.reg[7], this.reg.PC + (IR >>> 6) + 1);
        this.reg.PC = this.reg[operandA];
        addLen = false;
        break;

      case RET:
        this.reg.PC = this.ram.read(this.reg[7]);
        this.reg[7]++;
        addLen = false;
        break;

      case ADD:
        this.reg[operandA] = this.reg[operandA] * 2;
        break;

      case PRN:
        console.log(this.reg[operandA]);
        break;

      case HLT:
        this.stopClock();
        break;

      default:
        console.log(
          `error: unknown instruction at PC: ${
            this.reg.PC
          }. Return instruction: ${this.ram.read(this.reg.PC)}`
        );
        this.stopClock();
        break;
    }

    // Increment the PC register to go to the next instruction. Instructions
    // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
    // instruction byte tells you how many bytes follow the instruction byte
    // for any particular instruction.
    // !!! IMPLEMENT ME

    //&& get instruction length from current IR (instruction) from the first 2 bits
    if (addLen) {
      let instructionLen = (IR >>> 6) + 1;
      this.reg.PC += instructionLen;
    }
  }
}

module.exports = CPU;
