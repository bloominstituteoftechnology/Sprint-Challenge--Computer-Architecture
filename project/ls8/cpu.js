/**
 * LS-8 v2.0 emulator skeleton code
 */

// Constants for instructions
const ADD = 0b10101000;
const AND = 0b10110011;
const CALL = 0b01001000;
const CMP = 0b10100000;
const DEC = 0b01111001;
const DIV = 0b10101011;
const HLT = 0b00000001;
const INC = 0b01111000;
const IRET = 0b00001011;
const JEQ = 0b01010001;
const JGT = 0b01010100;
const JLT = 0b01010011;
const JMP = 0b01010000;
const JNE = 0b01010010;
const LD = 0b10011000;
const LDI = 0b10011001;
const MOD = 0b10101100;
const MUL = 0b10101010;
const NOP = 0b00000000;
const NOT = 0b01110000;
const OR = 0b10110001;
const POP = 0b01001100;
const PRA = 0b01000010;
const PRN = 0b01000011;
const PUSH = 0b01001101;
const RET = 0b00001001;
const ST = 0b10011010;
const SUB = 0b10101001;
const XOR = 0b10110010;

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
    //SP
    this.reg[7] = 0xf4;

    // Special-purpose registers
    this.PC = 0; // Program Counter
    this.FL = 0b00000000;
  }

  // Set Flag
  setFlag(flag) {
    const flags = {
      EQ: 0b00000001,
      GT: 0b00000010,
      LT: 0b00000100
    };
    this.FL = 0b00000000;
    this.FL = this.FL | flags[flag];
    // console.log("FLAG: ", this.FL);
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

  //Helper functions
  printStack() {
    console.log("Stack: ", this.ram.read(this.reg[7]));
  }

  //Instruction functions
  and(x, y) {
    this.reg[x] = this.reg[x] & this.reg[y];
  }
  call(register) {
    // console.log("RAM: ", this.ram.mem.slice(0, 30));
    // console.log("Next Instruction: ", this.ram.read(this.PC + 2));
    this.reg[7]--;
    this.ram.write(this.reg[7], this.PC + 2);
    this.PC = this.reg[register];
  }
  cmp(regA, regB) {
    if (this.reg[regA] === this.reg[regB]) {
      this.setFlag("EQ");
    } else if (this.reg[regA] < this.reg[regB]) {
      this.setFlag("LT");
    } else {
      this.setFlag("GT");
    }
  }
  dec(reg) {
    this.reg[reg]--;
  }
  hlt() {
    this.stopClock();
    return;
  }
  inc(reg) {
    this.reg[reg]++;
  }
  int(reg) {
    // Issue the interrupt number stored in the given register.
    // This will set the _n_th bit in the IS register to the value in the given register.
  }
  iret() {
    // Return from an interrupt handler.
    // The following steps are executed:
    // Registers R6-R0 are popped off the stack in that order.
    // The FL register is popped off the stack.
    // The return address is popped off the stack and stored in PC.
    // Interrupts are re-enabled
  }
  jeq(reg) {
    // If equal flag is set (true), jump to the address stored in the given register.
    if (this.FL == 1) {
      this.jmp(reg);
      // console.log("Jumped EQ");
    } else {
      // console.log("Not EQ");
      this.PC += 2;
    }
  }
  jgt(reg) {
    // If greater-than flag is set (true), jump to the address stored in the given register.
    if (this.FL == 0b00000010) {
      this.jmp(reg);
      // console.log("Jumped GT");
    } else {
      this.PC += 2;
    }
  }
  jlt(reg) {
    // If less-than flag is set (true), jump to the address stored in the given register.
    if (this.FL == 4) {
      this.jmp(reg);
      // console.log("Jumped LT");
    } else {
      this.PC += 2;
    }
  }
  jmp(reg) {
    //
    this.PC = this.reg[reg];
    // console.log("Jumped for no reason");
  }
  jne(reg) {
    // If E flag is clear (false, 0), jump to the address stored in the given register.
    if (this.FL != 1) {
      this.jmp(reg);
      // console.log("Jumped NE");
    } else {
      this.PC += 2;
    }
  }
  ld(regA, regB) {
    //     Loads registerA with the value at the address stored in registerB.
    // This opcode reads from memory.
  }
  ldi(register, integer) {
    this.reg[register] = integer;
  }
  not(reg) {}
  pop(reg) {
    // this.printStack();
    this.reg[reg] = this.ram.read(this.reg[7]);
    this.ram.write(this.reg[7], null);
    this.reg[7]++;
    // this.printStack();
  }
  pra(reg) {}
  prn(register) {
    console.log(this.reg[register]);
  }
  push(reg) {
    // this.printStack();
    // console.log("Pre SP", this.reg[7]);
    this.reg[7]--;
    // console.log("POST SP: ", this.reg[7]);
    this.ram.write(this.reg[7], this.reg[reg]);
    // this.printStack();
  }
  ret() {
    this.PC = this.ram.read(this.reg[7]);
    this.reg[7]++;
  }
  st(regA, regB) {
    this.ram.write(regA, regB);
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
        this.reg[regA] = this.reg[regA] + this.reg[regB];
        break;
      case "DIV":
        this.reg[regA] = this.reg[regA] / this.reg[regB];
        break;
      case "MUL":
        this.reg[regA] = this.reg[regA] * this.reg[regB];
        break;
      case "MOD":
        this.reg[regA] = this.reg[regA] % this.reg[regB];
        break;
      case "SUB":
        this.reg[regA] -= this.reg[regB];
        break;
    }
  }

  /**
   * Advances the CPU one cycle
   */
  tick() {
    // console.log("PC: ", this.PC);
    // console.log("SP: ", this.reg[7]);
    // console.log(LDI);
    // Load the instruction register (IR--can just be a local variable here)
    // from the memory address pointed to by the PC. (I.e. the PC holds the
    // index into memory of the instruction that's about to be executed
    // right now.)
    let IR = this.ram.read(this.PC);

    // !!! IMPLEMENT ME

    // Debugging output
    // console.log(`${this.PC}: ${IR.toString(2)}`, "REGISTER: ", this.reg);

    // Get the two bytes in memory _after_ the PC in case the instruction
    // needs them.

    // !!! IMPLEMENT ME
    let next1 = this.ram.read(this.PC + 1);
    let next2 = this.ram.read(this.PC + 2);

    // Execute the instruction. Perform the actions for the instruction as
    // outlined in the LS-8 spec.

    switch (IR) {
      case ADD:
        this.alu("ADD", next1, next2);
        break;
      case AND:
        this.and(next1, next2);
        break;
      case CMP:
        // console.log("Comparing");
        this.cmp(next1, next2);
        break;
      case HLT:
        this.hlt();
        break;
      case LDI:
        this.ldi(next1, next2);
        break;
      case MUL:
        this.alu("MUL", next1, next2);
        break;
      case POP:
        this.pop(next1);
        break;
      case PRN:
        this.prn(next1);
        break;
      case PUSH:
        this.push(next1);
        break;
      case CALL:
        this.call(next1);
        break;
      case RET:
        this.ret();
        break;
      case JEQ:
        // console.log("Calling JEQ");
        this.jeq(next1);
        break;
      case JLT:
        this.jlt(next1);
        break;
      case JGT:
        this.jgt(next1);
        break;
      case JMP:
        this.jmp(next1);
        break;
      case JNE:
        this.jne(next1);
        break;
      default:
        console.log("Unknown instruction. Halting");
        this.hlt();
    }

    // !!! IMPLEMENT ME

    // Increment the PC register to go to the next instruction. Instructions
    // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
    // instruction byte tells you how many bytes follow the instruction byte
    // for any particular instruction.
    if (
      IR !== CALL &&
      IR !== RET &&
      IR !== JMP &&
      IR !== JNE &&
      IR !== JEQ &&
      IR !== JLT &&
      IR !== JGT
    ) {
      this.PC += (IR >> 6) + 1;
    }
    // console.log("PC after: ", this.PC);
    // !!! IMPLEMENT ME
  }
}

module.exports = CPU;
