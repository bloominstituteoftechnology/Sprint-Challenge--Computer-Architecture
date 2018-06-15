/**
 * LS-8 v2.0 emulator skeleton code
 */
const LDI = 0b10011001;
const PRN = 0b01000011;
const HLT = 0b00000001;
const MUL = 0b10101010;
const PUSH = 0b01001101;
const POP = 0b01001100;
const RET = 0b00001001;
const CALL = 0b01001000;

const ADD = 0b10101000;

const SP = 0x07; // Stack Pointer

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
    this.PC = 0; // Program Counter
    this.reg[SP] = 0xf4; // SP default value set to 0xf4 or 244
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
        // this.reg[regA] = (regA + regB) & 255;
        this.reg[regA] = this.reg[regA] + this.reg[regB];
        break;

      case "SUB":
        this.reg[regA] = (regA - regB) & 255;
        break;

      case "MUL":
        this.reg[regA] = this.reg[regA] * this.reg[regB];
        // this.reg[regA] = (regA * regB) & 255;
        break;

      case "DIV":
        this.reg[regA] = regA / regB;
        break;

      case "INC":
        return (regA = (regA + 1) & 0xff);

      case "DEC":
        return (regA = (regA - 1) & 0xff);

      // case "CMP":
      //   // !!! IMPLEMENT ME
      //   this.reg[regA] =
      //   break;
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
    const IR = this.ram.read(this.PC);

    // Debugging output
    console.log(this.reg);
    // console.log(`${this.PC}: ${IR.toString(2)}`);

    // Get the two bytes in memory _after_ the PC in case the instruction
    // needs them.
    // !!! IMPLEMENT ME
    const operandA = this.ram.read(this.PC + 1);
    const operandB = this.ram.read(this.PC + 2);

    // Execute the instruction. Perform the actions for the instruction as
    // outlined in the LS-8 spec.
    // !!! IMPLEMENT ME

    switch (IR) {
      case LDI:
        // Set the value in a register
        this.reg[operandA] = operandB;
        // this.PC += 3; // next instructions; disabled due to instLen
        break;

      case PRN:
        console.log(this.reg[operandA]);
        // this.PC += 2; disabled due to instLen
        break;

      case HLT:
        this.stopClock();
        // this.PC += 1; disabled due to instLen
        break;

      case MUL:
        this.alu("MUL", operandA, operandB);
        break;

      case PUSH:
        this.reg[SP] = this.alu("DEC", this.reg[SP]); // returns address to RAM
        this.poke(this.reg[SP], this.reg[operandA]); // write to RAM address specified in SP slot
        break;

      case POP:
        // read value from RAM address specified in SP slot
        this.reg[operandA] = this.ram.read(this.reg[SP]); // read from RAM address specified in SP slot, then set register to read value.
        this.reg[SP] = this.alu("INC", this.reg[SP]);
        break;

      case CALL:
        // let caller = this.reg[operandA];
        // this.reg[operandA] = this.PC + 2;
        // this.tick.PUSH;
        // this.PC = caller - 2;

        // console.log("call");
        let caller = this.reg[operandA];
        this.poke(this.reg[SP], this.PC + 2);
        this.PC = caller - 2;
        break;

      case RET:
        // console.log("ret");
        let returner = this.ram.read(0xf4); // calls the top-most RAM address, which in our case is 244
        this.PC = returner - 1;

        break;

      case ADD:
        this.alu("ADD", operandA, operandB);
        break;

      default:
        console.log("unknown instruction: ", +IR.toString(2));
        this.stopClock();
        return;
    }

    // Increment the PC register to go to the next instruction. Instructions
    // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
    // instruction byte tells you how many bytes follow the instruction byte
    // for any particular instruction.
    // !!! IMPLEMENT ME

    const instLen = (IR >> 6) + 1;
    this.PC += instLen;
  }
}

module.exports = CPU;
