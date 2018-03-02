/**
 * LS-8 v2.0 emulator skeleton code
 */
const fs = require('fs');
const SP_ADDR = 7;
const BT = {
  // Instructions - Ref LS8-SP_ADDREC.md
  ADD: 0b10101000, // :144 tested etisdew
  AND: 0b10110011, // :155
  CALL: 0b01001000, // :167
  CMP: 0b10100000, // :182
  DEC: 0b01111001, // :201 tested etisdew
  DIV: 0b10101011, // :212 tested etisdew
  HLT: 0b00000001, // :227
  INC: 0b01111000, // :238 tested etisdew
  INT: 0b01001010, // :249
  IRET: 0b00001011, // :263
  JEQ: 0b01010001, // :281
  JGT: 0b01010100, // :292
  JLT: 0b01010011, // :304
  JMP: 0b01010000, // :316
  JNE: 0b01010010, // :329
  LD: 0b10011000, // :341
  LDI: 0b10011001, // :354
  MOD: 0b10101100, // :365 tested etisdew
  MUL: 0b10101010, // :380 tested etisdew
  NOP: 0b00000000, // :391
  NOT: 0b01110000, // :402
  OR: 0b10110001, // :413
  POP: 0b01001100, // :425
  PRA: 0b01000010, // :439
  PRN: 0b01000011, // :453
  PUSH: 0b01001101, // :467
  RET: 0b00001001, // :482
  ST: 0b10011010, // :495
  SUB: 0b10101001, // :508 tested etisdew
  XOR: 0b10110010 // :520
}


class CPU {

  /**
   * Initialize the CPU
   */
  constructor(ram = 256) {
    this.ram = ram;
    this.reg = new Array(8).fill(0); // General-purpose registers

    // Special-purpose registers
    this.reg.PC = 0; // Program Counter
    this.reg.IR = 0; // Instruction Register
    this.reg[SP_ADDR] = 0xf4;
    this.setupBranchTable();
  }

	/**
	 * Sets up the branch table
     * @param {void} refactor to take BT
	 */
  setupBranchTable() {
    let bt = {};
    // I think there is an improvement to be made here to remove the
    // multiple declarations.
    bt[BT.ADD] = this.ADD.bind(this); // works
    bt[BT.AND] = this.AND.bind(this); // works
    bt[BT.CALL] = this.CALL.bind(this); // test
    bt[BT.CMP] = this.CMP.bind(this); // works
    bt[BT.DEC] = this.DEC.bind(this); // works
    bt[BT.DIV] = this.DIV.bind(this); // works
    bt[BT.HLT] = this.HLT.bind(this); // works
    bt[BT.INC] = this.INC.bind(this); // works
    // bt[BT.INT] = this.INT.bind(this); //    make
    // bt[BT.IRET] = this.IRET.bind(this); //  make
    bt[BT.JEQ] = this.JEQ.bind(this); //    make
    bt[BT.JGT] = this.JGT.bind(this); //    make
    bt[BT.JLT] = this.JLT.bind(this); //    make
    bt[BT.JMP] = this.JMP.bind(this); //    make
    bt[BT.JNE] = this.JNE.bind(this); //    make
    // bt[BT.LD] = this.LD.bind(this); //    make
    bt[BT.LDI] = this.LDI.bind(this); // works
    bt[BT.MOD] = this.MOD.bind(this); // works
    bt[BT.MUL] = this.MUL.bind(this); // works
    bt[BT.NOP] = this.NOP.bind(this); // works
    bt[BT.NOT] = this.NOT.bind(this); // works
    bt[BT.OR] = this.OR.bind(this); // works
    bt[BT.POP] = this.POP.bind(this); //    test
    // bt[BT.PRA] = this.PRA.bind(this); //    make
    bt[BT.PRN] = this.PRN.bind(this); // works
    bt[BT.PUSH] = this.PUSH.bind(this); //    test
    bt[BT.RET] = this.RET.bind(this); //    test
    // bt[BT.ST] = this.ST.bind(this); //    make
    bt[BT.SUB] = this.SUB.bind(this); // works
    bt[BT.XOR] = this.XOR.bind(this); // works
    // Establishes that this.branchTable is initialized with its op codes
    // as keys and functions as value.
    this.branchTable = bt;
  }

  /**
   * Store value in memory address, useful for program loading
   * @param address Memory address
   * @param value Value to place in memory address
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
   * stopClock
   * calls clearInterval with this.clock
   */
  stopClock() {
    clearInterval(this.clock);
  }

  /**
   * ALU functionality
   * 
   * op can be: ADD SUB MUL DIV INC DEC CMP MOD
   */
  alu(op, regA, regB) {
    switch (op) {
      case 'NOP': { break; }
      case 'ADD': { this.reg[regA] += this.reg[regB]; break; }
      case 'SUB': { this.reg[regA] -= this.reg[regB]; break; }
      case 'MUL': { this.reg[regA] *= this.reg[regB]; break; }
      case 'DIV': { this.reg[regA] /= this.reg[regB]; break; }
      case 'MOD': { this.reg[regA] %= this.reg[regB]; break; }
      case 'INC': { this.reg[regA] += 1; break; }
      case 'DEC': { this.reg[regA] -= 1; break; }
      case 'NOT': { this.reg[regA] = ~this.reg[regA]; break; }
      case 'AND': { this.reg[regA] &= this.reg[regB]; break; }
      case 'OR': { this.reg[regA] |= this.reg[regB]; break; }
      case 'XOR': { this.reg[regA] ^= this.reg[regB]; break; }
      case 'CMP': {
        let flag = 0b000;
        if (this.reg[regA] === this.reg[regB]) {
          flag += 1
        }
        if (this.reg[regA] < this.reg[regB]) {
          flag += 2
        }
        if (this.reg[regA] > this.reg[regB]) {
          flag += 4
        }
        return flag;
      }
      case 'JEQ': if (this.flag === 1)
      case 'JGT': if (this.flag === 2)
      case 'JLT': if (this.flag === 4)
      case 'JNE': if (this.flag === 0)
    }
  }

  /**
   * Advances the CPU one cycle
   */
  tick() {
    // Load the instruction register (IR) from the current PC
    // !!! IMPLEMENT ME
    this.reg.IR = this.ram.read(this.reg.PC);
    let operandA = this.ram.read(this.reg.PC + 1);
    let operandB = this.ram.read(this.reg.PC + 2);
    // Debugging output
    console.log(
      `PC: ${this.reg.PC} RAMREAD: ${this.reg}
      IR: ${this.reg.IR} ?MEM/PROP: ${operandA} ?MEM/PROP: ${operandB}
      SP_ADDR: ${this.reg[SP_ADDR]}`
    );
    // Based on the value in the Instruction Register, locate the
    // appropriate hander in the branchTable        
    let handler = this.branchTable[this.reg.IR];
    // Check that the handler is defined, halt if not
    if (typeof handler === 'function') {
      handler(operandA, operandB);
      let bitMaskedThing = (((this.reg.IR >> 6) & 3) + 1);
      this.reg.PC += bitMaskedThing;
    } else {
      if (!!handler) {
        this.reg.PC = handler;
      } else {
        this.stopClock();
      }
    }
  }

  // INSTRUCTION HANDLER CODE:
  // Gate Operations
  NOT(regA) { this.alu('NOT', regA) }
  AND(regA, regB) { this.alu('AND', regA, regB) }
  OR(regA, regB) { this.alu('OR', regA, regB) }
  XOR(regA, regB) { this.alu('XOR', regA, regB) }
  // Math Operations
  INC(regA) { this.alu('INC', regA) }
  DEC(regA) { this.alu('DEC', regA) }
  ADD(regA, regB) { this.alu('ADD', regA, regB) }
  SUB(regA, regB) { this.alu('SUB', regA, regB) }
  MUL(regA, regB) { this.alu('MUL', regA, regB) }
  DIV(regA, regB) { this.alu('DIV', regA, regB) }
  MOD(regA, regB) { this.alu('MOD', regA, regB) }
  CMP(regA, regB) { this.flag = this.alu('CMP', regA, regB) }
  // Core Operations
  NOP() { this.alu('NOP') }
  HLT() { this.stopClock() }
  LDI(regNum, value) { this.reg[regNum] = value }
  PRN(regA) { console.log(this.reg[regA]) }
  // Stack Operations
  stackHelper(op, value) {
    switch (op) {
      case 'CALL':
      case 'PUSH': this.ram.write(--this.reg[SP_ADDR], value); break;
      case 'POP': this.reg[regNum] = this.ram.read(this.reg[SP_ADDR]++); break;
      case 'RET': this.reg.PC = this.ram.read(this.reg[SP_ADDR]++); console.log(this.reg.PC); break;
      default: console.warn('Invalid SP_ADDR operation. See methods: PUSH POP CALL');
    }
  }
  CALL(regNum) { this.stackHelper('CALL', this.reg.PC + 2); return this.reg[regNum]; }
  PUSH(regNum) { this.stackHelper('PUSH', this.reg[regNum]) }
  POP(regNum) { this.stackHelper('POP', this.reg[regNum]) }
  RET() { this.stackHelper('RET') }

  JMP(regA) {}
  JEQ(regA) { this.alu('JEQ', regA) }
  JGT(regA) { this.alu('JGT', regA) }
  JLT(regA) { this.alu('JLT', regA) }
  JNE(regA) { this.alu('JNE', regA) }

}


module.exports = CPU;
