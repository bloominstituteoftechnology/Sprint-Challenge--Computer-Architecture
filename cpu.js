const HLT = 0b00000001,
      PRN = 0b01000011,
      LDI = 0b10011001,
      MUL = 0b10101010,
      ADD = 0b10101000,
      AND = 0b10110011,
      CALL= 0b01001000,
      CMP = 0b10100000,
      DEC = 0b01111001,
      DIV = 0b10101011,
      INC = 0b01111000,
      INT = 0b01001010,
      IRET= 0b00001011,
      JEQ = 0b01010001,
      JGT = 0b01010100,
      JLT = 0b01010011,
      JMP = 0b01010000,
      JNE = 0b01010010,
      LD  = 0b10011000,
      MOD = 0b10101100,
      NOP = 0b00000000,
      NOT = 0b01110000,
      OR  = 0b10110001,
      POP = 0b01001100,
      PRA = 0b01000010,
      PUSH= 0b01001101,
      RET = 0b00001001,
      ST  = 0b10011010,
      SUB = 0b10101001,
      XOR = 0b10110010;

const FLAG_EQUAL = 0;
const FLAG_GREAT = 1;
const FLAG_LESS  = 2;

const SP = 7;
const IS = 6;
const IM = 5;

class CPU {
  constructor(ram) {
    this.ram = ram;
    this.reg = new Array(8).fill(0); // register R0-R7
    this.PC = 0; // program counter - holds the address of the currently executed instruction
    this.FL = 0; // flags register - holds the current flags status
    this.reg[SP] = 0xF4; // stores the address (in ram) of the most recently pushed item in the stack
  }

  /* Store a byte of data in the memory address */
  writeToRAM(address, value) {
    this.ram.write(address, value);
  }

  /* Start the clock on the CPU */
  startClock() {
    this.clock = setInterval(() => this.tick(), 1);
  }

  /* Stop the clock. Halt everything. */
  stopClock() {
    clearInterval(this.clock);
  }

  /* ARITHMETIC LOGIC UNIT - the part of the CPU that strictly handles basic arithmetic and comparisions */
  ALU(operation, regA, regB) {
    switch(operation) {
      case "ADD": // add
        this.reg[regA] += this.reg[regB];
        break;

      case "SUB": // subtract
        this.reg[regA] -= this.reg[regB];
        break;

      case "MUL": // multiply
        this.reg[regA] *= this.reg[regB];
        break;

      case "DIV": // divide
        this.reg[regA] /= this.reg[regB];
        break;

      case "MOD": // modulo
        this.reg[regA] %= this.reg[regB];
        break;

      case "AND":
        this.reg[regA] &= this.reg[regB];
        break;

      case "OR":
        this.reg[regA] |= this.reg[regB];
        break;

      case "NOT":
        this.reg[regA] = ~this.reg[regA];
        break;

      case "XOR":
        this.reg[regA] ^= this.reg[regB];
        break;

      case "CMP": // compare
        this.compare(regA, regB);
        break;

      case "DEC": // decrement
        this.reg[regA]--;
        break;

      case "INC": // increment
        this.reg[regA]++;
        break;

      default:
        console.log(`Error. ALU case ${operation} is not defined.`);
        this.stopClock();
        break;
    }
  }

  /* Advances the CPU one cycle */
  tick() {
    // If this.PC is not set, incrementPC remains true.
    // Otherwise, incrementPC will be set to false, and the value of this.PC will be set manually.
    let incrementPC = true;

    // Instruction Register - contains a copy of the currently executing instruction
    const IR = this.ram.read(this.PC);

    // Retrieve the two bytes in memory after the PC (incase the instruction needs them)
    const operandA = this.ram.read(this.PC + 1);
    const operandB = this.ram.read(this.PC + 2);

    // Execute the instruction. Perform the necessary actions as outlined in the spec.
    // for this sprint you need: LDI, CMP, JEQ, PRN, JNE, JMP
    switch(IR) {
      case ADD: 
        this.ALU("ADD", operandA, operandB);
        break;

      case AND:
        this.ALU("AND", operandA, operandB);
        break;

      case CALL: // calls a subroutine at the address stored in the register ~ 01001000 00000rrr
        this.push(this.PC + 2);
        this.PC = this.reg[operandA];
        incrementPC = false;
        break;

      case CMP: // compare the value in two registers ~ 10100000 00000aaa 00000bbb
        this.ALU("CMP", operandA, operandB);
        break;

      case DEC: // decrement the value by 1 in the given register
        this.ALU("DEC", operandA);
        break;

      case DIV: // divide regA by regB and store the result in regA
        if (this.reg[operandB] === 0) {
          console.log("Invalid value. Divide case. Cannot divide by 0.");
          this.stopClock();
        } else {
          this.ALU("DIV", operandA, operandB);
        }
        break;

      case HLT: // halt the CPU and exit the emulator
        this.stopClock();
        break;

      case INC: // increment the value of regA by 1
        this.ALU("INC", operandA);
        break;

      case INT:
        console.log(`case INT - ${IR} - has not yet been defined.`);
        this.stopClock();
        break;

      case IRET:
        console.log(`case IRET - ${IR} - has not yet been defined.`);
        this.stopClock();
        break;

      case JEQ: // if an equal flag is set, jump to the address stored in the given register ~ 01010001 00000rrr
        if (this.getFlag(FLAG_EQUAL)) {
          this.PC = this.reg[operandA];
          incrementPC = false;
        }
        break;

      case JGT: // if the Greater Than flag is set, jump to the address stored in the given register ~ 01010100 00000rrr
        if (this.getFlag(FLAG_GREAT)) {
          this.PC = this.reg[operandA];
          incrementPC = false;
        }
        break;

      case JLT: // if the Less Than flag is set, jump to the address stored in the given register ~ 01010011 00000rrr
        if (this.getFlag(FLAG_LESS)) {
          this.PC = this.reg[operandA];
          incrementPC = false;
        }
        break;

      case JMP: // jump to the address stored in the given register ~ 01010000 00000rrr
        this.PC = this.reg[operandA]; // set the PC to the address stored in the given register
        incrementPC = false;
        break;

      case JNE: // if the equal flag is not set, jump to the address stored in the given register ~ 01010010 00000rrr
        if (!this.getFlag(FLAG_EQUAL)) {
          this.PC = this.reg[operandA];
          incrementPC = false;
        }
        break;

      case LD: // loads regA with the value at the address stored in regB
        console.log(`case LD - ${IR} - has not yet been defined.`);
        this.stopClock();
        break;

      case LDI: // set the value of a register to an integer ~ 10011001 00000rrr iiiiiiii
        this.reg[operandA] = operandB;
        break;

      case MOD:
        if (this.reg[operandB] === 0) {
          console.log("Invalid value. Modulo case. Cannot divide by 0.");
          this.stopClock();
        } else {
          this.ALU("MOD", operandA, operandB);
        }
        break;

      case MUL:
        this.ALU("MUL", operandA, operandB);
        break;

      case NOP: // no operation - do nothing for this instruction ~ 00000000
        break;

      case NOT: // perform a bitwise-NOT on the value in the given register
        this.ALU("NOT", operandA, operandB);
        break;

      case OR: // perform a bitwise-OR between regA and regB, store the result in regA
        this.ALU("OR", operandA, operandB);
        break;

      case POP: // pop the value from the top of the stack into the given register ~ 01001100 00000rrr
        this.reg[operandA] = this.ram.read(this.reg[SP]);
        this.reg[SP]++;
        break;

      case PRA: // print the alpha character value stored in the given register ~ 01000010 00000rrr
        console.log(`case PRA - ${IR} - has not yet been defined.`);
        this.stopClock();
        break;
      
      case PRN: // print the numeric value stored in the given register ~ 01000011 00000rrr
        console.log(this.reg[operandA]);
        break;

      case PUSH: // push the given register onto the stack ~ 01001101 00000rrr
        this.push(this.reg[operandA]);
        break;

      case RET: // return from the subroutine
        this.PC = this.ram.read(this.reg[SP]);
        this.reg[SP]++;
        incrementPC = false;
        break;

      case ST: // store the value in regB in the address stored in regA ~ 10011010 00000aaa 00000bbb
        this.ram.write(this.reg[operandA], this.reg[operandB]);
        break;

      case SUB: // subtract the value in regB from regA and store it in regA
        this.ALU("SUB", operandA, operandB);
        break;

      case XOR: // perform a bitwise-XOR between regA and regB and store the result in regA
        this.ALU("XOR", operandA, operandB);
        break;

      default:
        console.log(`Instruction ${IR} is not defined in tick().`);
        this.stopClock();
        break;
    }

    incrementPC ? this.PC += (IR >> 6) + 1 : null;
  }

  compare(regA, regB) {
    const equal   = +(this.reg[regA] === this.reg[regB]);
    const greater = +(this.reg[regA] > this.reg[regB]);
    const less    = +(this.reg[regA] < this.reg[regB]);

    const compEQ = eq => {
      if (eq) this.FL |= (1 << FLAG_EQUAL);
      else this.FL &= ~(1 << FLAG_EQUAL);
    }

    const compGT = gt => {
      if (gt) this.FL |= (1 << FLAG_GREAT);
      else this.FL &= ~(1 << FLAG_GREAT);
    }

    const compLT = lt => {
      if (lt) this.FL |= (1 << FLAG_LESS);
      else this.FL &= ~(1 << FLAG_LESS);
    }

    compEQ(equal);
    compGT(greater);
    compLT(less);
  }

  getFlag(flag) {
    return (this.FL & (1 << flag)) >> flag;
  }

  push(value) {
    this.reg[SP]--;
    this.ram.write(this.reg[SP], value);
  }
}

module.exports = CPU;