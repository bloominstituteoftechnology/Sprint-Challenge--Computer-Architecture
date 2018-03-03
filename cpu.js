/**
 * LS-8 v2.0 emulator skeleton code
 */
const fs = require('fs');
const SP_ADDR = 7;
const DEFAULT_OP_SET = {
  // Instructions - Ref LS8-SP_ADDREC.md Starting on ALU...
  // Test 0: Defined in ...
   LD:  0b10011000, // :341
  LDI:  0b10011001, // :354
   ST:  0b10011010, // :495
  // Test 1: Defined in ...
  CMP:  0b10100000, // :182
  // Test 2: Defined in ...
  ADD:  0b10101000, // :144 tested etisdew
  SUB:  0b10101001, // :508 tested etisdew
  MUL:  0b10101010, // :380 tested etisdew
  DIV:  0b10101011, // :212 tested etisdew
  MOD:  0b10101100, // :365 tested etisdew
  // Test 3: Defined in ...
   OR:  0b10110001, // :413
  XOR:  0b10110010, // :520
  AND:  0b10110011, // :155
  // Test 4: Defined in ...
  PRA:  0b01000010, // :439
  PRN:  0b01000011, // :453
  // Test 5: Defined in ...
 CALL:  0b01001000, // :167
  INT:  0b01001010, // :249
  POP:  0b01001100, // :425
 PUSH:  0b01001101, // :467
  // Test 6: Defined in ...
  JMP:  0b01010000, // :316
  JEQ:  0b01010001, // :281
  JNE:  0b01010010, // :329
  JLT:  0b01010011, // :304
  JGT:  0b01010100, // :292
  // Test 7: Defined in ...
  NOT:  0b01110000, // :402
  INC:  0b01111000, // :238 tested etisdew
  DEC:  0b01111001, // :201 tested etisdew
  // Test 8: Defined in ...
  NOP:  0b00000000, // :391
  HLT:  0b00000001, // :227
  RET:  0b00001001, // :482
 IRET:  0b00001011, // :263
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
    this.flag = 0b000;
    this.setupBranchTable(DEFAULT_OP_SET);
    // console.log(this.BT)
  }

	/**
	 * Sets up the branch table
   * @param { Object } BT provide the binary dictionary for mapping to function calls.
   * 
   */
  setupBranchTable(BT) {
    const bt = {};
    bt[BT.ADD]  = this.ADD.bind(this); // works
    bt[BT.AND]  = this.AND.bind(this); // works
    bt[BT.CALL] = this.CALL.bind(this); // test
    bt[BT.CMP]  = this.CMP.bind(this); // works
    bt[BT.DEC]  = this.DEC.bind(this); // works
    bt[BT.DIV]  = this.DIV.bind(this); // works
    bt[BT.HLT]  = this.HLT.bind(this); // works
    bt[BT.INC]  = this.INC.bind(this); // works
    // bt[BT.INT]   = this.INT.bind(this); //    make
    // bt[BT.IRET]  = this.IRET.bind(this); //  make
    // bt[BT.JEQ]  = this.JEQ.bind(this); //    make
    // bt[BT.JGT]  = this.JGT.bind(this); //    make
    // bt[BT.JLT]  = this.JLT.bind(this); //    make
    // bt[BT.JMP]  = this.JMP.bind(this); //    make
    // bt[BT.JNE]  = this.JNE.bind(this); //    make
    // bt[BT.LD]  = this.LD.bind(this); //    make
    bt[BT.LDI]  = this.LDI.bind(this); // works
    bt[BT.MOD]  = this.MOD.bind(this); // works
    bt[BT.MUL]  = this.MUL.bind(this); // works
    bt[BT.NOP]  = this.NOP.bind(this); // works
    bt[BT.NOT]  = this.NOT.bind(this); // works
    bt[BT.OR]   = this.OR.bind(this); // works
    bt[BT.POP]  = this.POP.bind(this); //    test
    // bt[BT.PRA]   = this.PRA.bind(this); //    make
    bt[BT.PRN]  = this.PRN.bind(this); // works
    bt[BT.PUSH] = this.PUSH.bind(this); //    test
    bt[BT.RET]  = this.RET.bind(this); //    test
    // bt[BT.ST]  = this.ST.bind(this); //    make
    bt[BT.SUB]  = this.SUB.bind(this); // works
    bt[BT.XOR]  = this.XOR.bind(this); // works
    this.branchTable = bt;
  }

  poke(address, value) { this.ram.write(address, value) }
  startClock() { this.clock = setInterval(_ => { this.tick() }); }
  stopClock() { clearInterval(this.clock) }

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
      
      case 'OR':  { this.reg[regA] |= this.reg[regB]; break; }
      case 'XOR': { this.reg[regA] ^= this.reg[regB]; break; }
      case 'NOT': { this.reg[regA] = ~this.reg[regA]; break; }
      case 'AND': { this.reg[regA] &= this.reg[regB]; break; }
      
      case 'JEQ': (this.flag === 0b001);
      case 'JGT': (this.flag === 0b010);
      case 'JLT': (this.flag === 0b100);
      case 'JNE': (this.flag !== 0b001);
      
      case 'CMP': {
        if (this.reg[regA] === this.reg[regB]) { this.flag += 0b001; }
        if (this.reg[regA] < this.reg[regB]) { this.flag += 0b010; }
        if (this.reg[regA] > this.reg[regB]) { this.flag += 0b100; }
      }
    }
  }

  /**
   * Advances the CPU one cycle
   */
  tick() {
    // Load the instruction register (IR) from the current PC
    this.reg.IR = this.ram.read(this.reg.PC);
    let operandA = this.ram.read(this.reg.PC + 1);
    let operandB = this.ram.read(this.reg.PC + 2);

    // Debugging output
    console.log(`PC: ${this.reg.PC} RAMREAD: ${this.reg} IR: ${this.reg.IR} ?MEM/PROP: ${operandA} ?MEM/PROP: ${operandB} SP_ADDR: ${this.reg[SP_ADDR]}`);

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
  
  // Debugging output
  // INSTRUCTION HANDLER CODE:
  // Gate Operations
  NOT(regA)       { this.alu('NOT', regA) }
  AND(regA, regB) { this.alu('AND', regA, regB) }
  OR(regA, regB)  { this.alu('OR', regA, regB) }
  XOR(regA, regB) { this.alu('XOR', regA, regB) }
  // Math Operations
  INC(regA)       { this.alu('INC', regA) }
  DEC(regA)       { this.alu('DEC', regA) }
  ADD(regA, regB) { this.alu('ADD', regA, regB) }
  SUB(regA, regB) { this.alu('SUB', regA, regB) }
  MUL(regA, regB) { this.alu('MUL', regA, regB) }
  DIV(regA, regB) { this.alu('DIV', regA, regB) }
  MOD(regA, regB) { this.alu('MOD', regA, regB) }
  CMP(regA, regB) { this.flag = 0b000; this.flag = this.alu('CMP', regA, regB); }
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

  JMP(regNum) { this.reg.IR = regNum }
  JEQ(regNum) { if (this.alu('JEQ')) this.reg.IR = regNum; }
  JGT(regNum) { if (this.alu('JGT')) this.reg.IR = regNum; }
  JLT(regNum) { if (this.alu('JLT')) this.reg.IR = regNum; }
  JNE(regNum) { if (this.alu('JNE')) this.reg.IR = regNum; }
}


module.exports = CPU;
