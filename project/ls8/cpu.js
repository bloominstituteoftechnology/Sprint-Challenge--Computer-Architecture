/**
 * LS-8 v2.0 emulator skeleton code
 */

/**
 * Class for simulating a simple Computer (CPU & memory)
 */

const SP = 7; // Stack Pointer 

class CPU {
  /**
   * Initialize the CPU
   */
  constructor(ram) {
    this.ram = ram;
    this.reg = new Array(8).fill(0); // General-purpose registers R0-R7

    // Special-purpose registers
    this.reg.PC = 0; // Program Counter
    this.reg[SP] = 0xf4;
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
        this.reg[regA] *= this.reg[regB];
        break;
      case 'ADD':
        this.reg[regA] += this.reg[regB];
        break;
      default:
        break;
    }
  }

  /**
   * Advances the CPU one cycle
   */
  tick() {

    const ADD = 0b10101000;
    const LDI = 0b10011001;
    const HLT = 0b00000001;
    const PRN = 0b01000011;
    const MUL = 0b10101010;
    const POP = 0b01001100;
    const PUSH = 0b01001101;
    const CALL = 0b01001000;
    const RET = 0b00001001;

    // Load the instruction register (IR--can just be a local variable here)
    // from the memory address pointed to by the PC. (I.e. the PC holds the
    // index into memory of the instruction that's about to be executed
    // right now.)

    // !!! IMPLEMENT ME
    let IR = this.ram.read(this.reg.PC);

    // Debugging output
    //console.log(`${this.reg.PC}: ${IR.toString(2)}`);

    // Get the two bytes in memory _after_ the PC in case the instruction
    // needs them.

    // !!! IMPLEMENT ME
    let operandA = this.ram.read(this.reg.PC + 1);
    let operandB = this.ram.read(this.reg.PC + 2);

    // Execute the instruction. Perform the actions for the instruction as
    // outlined in the LS-8 spec.

    // !!! IMPLEMENT ME

    // Add the`CMP` instruction and`equal` flag to your LS - 8.

    // Add the`JMP` instruction.

    // Add the`JEQ` and`JNE` instructions.

    const handle_LDI = (operandA, operandB) => {
      this.reg[operandA] = operandB;
    };

    const handle_PRN = operandA => {
      console.log(this.reg[operandA]);
    };

    const handle_MUL = (operandA, operandB) => {
      this.alu('MUL', operandA, operandB);
    };

    const handle_ADD = (operandA, operandB) => {
      this.alu('ADD', operandA, operandB);
    }

    const handle_PUSH = operandA => {
      this.reg[SP]--;
      this.ram.write(this.reg[SP], this.reg[operandA]);
    };

    const handle_POP = operandA => {
      this.reg[operandA] = this.ram.read(this.reg[SP]);
      this.reg[SP]++;
    };

    const handle_RET = () => {
      this.reg.PC = this.ram.read(this.reg[SP]);
      this.reg[SP]++;
    }

    const handle_CALL = operandA => {
      this.reg[SP]--;
      this.ram.write(this.reg[SP], this.reg.PC + 2);
      this.reg.PC = this.reg[operandA];

      const handle_HLT = () => {
        this.stopClock();
      };

      const invalid_instruction = () => {
        console.log('invalid instruction: ' + IR.toString(2));
        this.stopClock();
      };

      const branchTable = {
        [ADD]: handle_ADD,
        [LDI]: handle_LDI,
        [PRN]: handle_PRN,
        [MUL]: handle_MUL,
        [PUSH]: handle_PUSH,
        [POP]: handle_POP,
        [HLT]: handle_HLT,
        [CALL]: handle_CALL,
        [RET]: handle_RET
      };

      branchTable[IR](operandA, operandB);

      // Increment the PC register to go to the next instruction. Instructions
      // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
      // instruction byte tells you how many bytes follow the instruction byte
      // for any particular instruction.

      // !!! IMPLEMENT ME

      if (IR !== CALL && IR !== RET) {
        let operandCount = (IR >>> 6) & 0b11;
        let totalInstructionLen = operandCount + 1;
        this.reg.PC += totalInstructionLen;
      }

    }
  }
}

module.exports = CPU;
