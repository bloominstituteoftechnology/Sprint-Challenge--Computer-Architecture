/**
 * LS-8 v2.0 emulator skeleton code
 */

const LDI = 0b10011001;
const PRN = 0b01000011;
const MUL = 0b10101010;
const HLT = 0b00000001;
const PUSH = 0b01001101;
const POP = 0b01001100;
const CALL = 0b01001000;
const JMP = 0b01010000;
const ADD = 0b10101000;
const RET = 0b00001001;

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
    this.reg[7] = 0xf4;

    this.PC = 0; // Program Counter
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
        // !!! IMPLEMENT ME
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
    const IR = this.ram.read(this.PC);
    // let SP = this.reg[7];

    // !!! IMPLEMENT ME

    // Debugging output
    // console.log(`${this.PC}: ${IR.toString(2)}`);

    // Get the two bytes in memory _after_ the PC in case the instruction
    // needs them.

    const operandA = this.ram.read(this.PC + 1);
    const operandB = this.ram.read(this.PC + 2);

    // !!! IMPLEMENT ME
    if (IR !== JMP && IR !== CALL && IR !== RET) {
      this.PC += 1 + (IR >> 6);
    }

    // Execute the instruction. Perform the actions for the instruction as
    // outlined in the LS-8 spec.

    switch (IR) {
      case LDI:
        this.reg[operandA] = operandB;
        break;
      case PRN:
        console.log(this.reg[operandA], 'PRN log');
        break;
      case MUL:
        this.reg[operandA] = this.reg[operandA] * this.reg[operandB];
        break;
      case HLT:
        this.stopClock();
        console.log('Operation completed Successfully!');
        break;
      case PUSH:
        this.reg[7]--;
        this.poke(this.reg[7], this.reg[operandA]);
        break;
      case POP:
        this.reg[operandA] = this.ram.read(this.reg[7]);
        ++this.reg[7];
        break;
      case CALL:
        this.reg[7]--;
        this.poke(this.reg[7], this.PC + 2);
        this.PC = this.reg[operandA];
        break;
      case JMP:
        this.PC = this.reg[operandA];

        break;
      case ADD:
        this.reg[operandA] += this.reg[operandB];
        break;

      case RET:
        this.PC = this.ram.read(this.reg[7]);
        ++this.reg[7];
        break;
      default:
        this.stopClock();
        console.log(
          `Unknown instruction at: ${this.PC - 1}: ${IR.toString(2)}`
        );
    }

    // !!! IMPLEMENT ME

    // Increment the PC register to go to the next instruction. Instructions
    // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
    // instruction byte tells you how many bytes follow the instruction byte
    // for any particular instruction.

    // this.PC += nextPC + 1;
    // console.log(this.PC);
    // console.log(this.ram.read(this.PC).toString(2));
    // console.log(this.ram.mem);
    // console.log(LDI);

    // !!! IMPLEMENT ME
    // this.stopClock();
  }
}

module.exports = CPU;
