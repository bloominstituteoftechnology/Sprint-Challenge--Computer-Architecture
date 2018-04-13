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

    // Special-purpose registers
    this.reg.PC = 0; // Program Counter
    this.reg[7] = this.ram.read(Number(0xf4.toString()));
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

    let callHandler;
    let interruptHandler;

    // Debugging output
    //console.log(`${this.reg.PC}: ${IR.toString(2)}`);

    // Get the two bytes in memory _after_ the PC in case the instruction
    // needs them.
    // !!! IMPLEMENT ME
    let operandA = this.ram.read(this.reg.PC + 1);
    let operandB = this.ram.read(this.reg.PC + 2);

    // Execute the instruction. Perform the actions for the instruction as
    // outlined in the LS-8 spec.
    const LDI = 0b10011001;
    const HLT = 0b00000001;
    const PRN = 0b01000011;
    const MUL = 0b10101010;
    const POP = 0b01001100;
    const PUSH = 0b01001101;
    const CALL = 0b01001000;
    const RET = 0b00001001;

    // switch (IR) {
    //   case LDI:
    //     this.reg[operandA] = operandB;
    //     break;
    //   case PRN:
    //     console.log(this.reg[operandA]);
    //     break;
    //   case MUL:
    //     this.alu('MUL', operandA, operandB);
    //     break;
    //   case PUSH:
    //     this.reg[7]--;
    //     this.ram.write(this.reg[7], this.reg[operandA]);
    //     break;
    //   case POP:
    //     this.reg[operandA] = this.ram.read(this.reg[7]);
    //     this.reg[7]++;
    //     break;
    //   case HLT:
    //     this.stopClock();
    //     break;
    //   default:
    //     console.log('invalid instruction: ' + IR.toString(2));
    //     this.stopClock();
    // }

    // !!! IMPLEMENT ME

    const handle_LDI = (operandA, operandB) => {
      this.reg[operandA] = operandB;
    };

    const handle_PRN = operandA => {
      console.log(this.reg[operandA]);
    };

    const handle_MUL = (operandA, operandB) => {
      this.alu('MUL', operandA, operandB);
    };

    const handle_PUSH = operandA => {
      this.reg[7]--;
      this.ram.write(this.reg[7], this.reg[operandA]);
    };

    const handle_POP = operandA => {
      this.reg[operandA] = this.ram.read(this.reg[7]);
      this.reg[7]++;
    };

    const handle_RET = () => {
      callHandler = this.ram.read(this.reg[7]);
      this.reg[7] += 1;
      return callHandler;
    }
    const handle_CALL = () => {
      this.reg[7] -= 1;
      this.ram.write(this.reg[7], this.reg.PC + (IR >>> 6) + 1)
      callHandler = this.reg[operandA];
      return callHandler;

      const handle_CALL = operandA => {
        // --this.reg[7];
        // this.ram.write(this.reg[7], this.reg.PC + 2)
        // this.reg.PC = this.reg[operandA];
        // console.log(this.reg[7])
        // console.log(this.reg.PC + 2)
        // console.log(this.reg[operandA])
        // return this.reg[operandA];
      };

      const handle_RET = () => {
        // this.reg.PC = this.ram.read(this.reg[7]);
        // this.reg[7]++;
      }

      const handle_HLT = () => {
        this.stopClock();
      };

      const invalid_instruction = () => {
        console.log('invalid instruction: ' + IR.toString(2));
        this.stopClock();
      };

      const branchTable = {
        [LDI]: handle_LDI,
        [PRN]: handle_PRN,
        [MUL]: handle_MUL,
        [PUSH]: handle_PUSH,
        [POP]: handle_POP,
        [HLT]: handle_HLT,
        [CALL]: handle_CALL,
        [RET]: handle_RET
      };

      if (Object.keys(branchTable).includes(IR.toString())) {
        branchTable[IR](operandA, operandB);
      } else {
        invalid_instruction();
      }

      // Increment the PC register to go to the next instruction. Instructions
      // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
      // instruction byte tells you how many bytes follow the instruction byte
      // for any particular instruction.

      // !!! IMPLEMENT ME
      let operandCount = (IR >>> 6) & 0b11;
      let totalInstructionLen = operandCount + 1;

      branchTable[IR](operandA, operandB);
      if (interruptHandler) {
        this.reg.PC = interruptHandler;
      } else if (callHandler) {
        this.reg.PC = callHandler;
      } else {
        this.reg.PC += (IR >>> 6) + 1;
      }

      //   switch (IR) {
      //     case CALL:
      //     default:
      //       this.reg.PC += totalInstructionLen;
      //   }
      // }
    }
  }
}

module.exports = CPU;
