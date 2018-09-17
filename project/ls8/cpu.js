// Constant variables associated with intruction bytes
const instructions = {
    '0b10101000': 'ADD',
    '0b10110011': 'AND',
    '0b01001000': 'CALL',
    '0b10100000': 'CMP',
    '0b01111001': 'DEC',
    '0b10101011': 'DIV',
    '0b00000001': 'HLT',
    '0b01111000': 'INC',
    '0b01001010': 'INT',
    '0b00001011': 'IRET',
    '0b01010001': 'JEQ',
    '0b01010100': 'JGT',
    '0b01010011': 'JLT',
    '0b01010000': 'JMP',
    '0b01010010': 'JNE',
    '0b10011000': 'LD',
    '0b10011001': 'LDI',
    '0b10101100': 'MOD',
    '0b10101010': 'MUL',
    '0b00000000': 'NOP',
    '0b01110000': 'NOT',
    '0b10110001': 'OR',
    '0b01001100': 'POP',
    '0b01000010': 'PRA',
    '0b01000011': 'PRN',
    '0b01001101':'PUSH',
    '0b00001001': 'RET',
    '0b10011010': 'ST',
    '0b10101001': 'SUB',
    '0b10110010': 'XOR'
  }
  
  /**
   * Class for simulating a simple Computer (CPU & memory)
   */
  class CPU {
  
    /**
     * Initialize the CPU
     */
    constructor(ram) {
      this.ram = ram;
  
      // General-purpose registers R0-R7
      this.reg = new Array(8).fill(0);
      
      // Special-purpose registers
      this.PC = 0; // Program Counter
      this.SP = 244; // Stack Pointer
      this.FL = '0b00000000'; // CMP Flags(LGE)
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
        case 'ADD':
          this.reg[regA] += this.reg[regB];
          break;
        case 'MUL':
          // !!! IMPLEMENT ME
          this.reg[regA] *= this.reg[regB];
          break;
      }
    }
  
    // Tick helper method
    jumpIf(condition, operand) {
      if (condition) {
        this.PC = this.reg[operand];
        this.pcAdvance = false;
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
  
      // Instruction Register
      const IR = this.ram.read(this.PC);
  
      // Debugging output
      // console.log(`${this.PC}: ${IR.toString(2)}`);
  
      // Get the two bytes in memory _after_ the PC in case the instruction
      // needs them.
      const operandA = this.ram.read(this.PC + 1);
      const operandB = this.ram.read(this.PC + 2);
  
      // Execute the instruction. Perform the actions for the instruction as
      // outlined in the LS-8 spec.
  
      const instBin = `0b${IR.toString(2).padStart(8, 0)}`;
      const argNum = Number(instBin) >> 6;
      // const isALU = (instBin && '0b00100000') >> 5;
      // const category = (instBin && '0b00011000') >> 3;
      // const instruction = (instBin && '0b00000111');
  
      this.pcAdvance = true; // Flag for PC advancment
      
      switch (instructions[instBin]) {
        case 'ADD':
          this.alu('ADD', operandA, operandB);
          break;
        case 'CALL':
          this.poke(--this.SP, this.PC + 2);
          this.PC = this.reg[operandA];
          this.pcAdvance = false;
          break;
        case 'CMP':
          const a = this.reg[operandA];
          const b = this.reg[operandB];
          // Check conditions and change to binary
          const cmp = [ a < b, a > b, a === b ]
            .map(val => String(Number(val))).join('');
          this.FL = '0b' + cmp.padStart(8, '0');
          break;
        case 'HLT':
          this.stopClock();
          break;
        case 'JEQ':
          this.jumpIf(this.FL[9] === '1', operandA);
          break;
        case 'JMP':
          this.jumpIf(true, operandA);
          break;
        case 'JNE':
          this.jumpIf(this.FL[9] === '0', operandA);
          break;
        case 'LDI':
          this.reg[operandA] = operandB;
          break;
        case 'MUL':
          this.alu('MUL', operandA, operandB);
          break;
        case 'NOP':
          break;
        case 'POP':
          this.reg[operandA] = this.reg[7];
          this.SP++;
          break;
        case 'PUSH':
          this.reg[7] = this.reg[operandA];
          this.poke(--this.SP, operandA);
          break;
        case 'PRN':
          console.log(this.reg[operandA]);
          break;
        case 'RET':
          this.PC = this.ram.read(this.SP++);
          this.pcAdvance = false;
          break;
        default:
          console.log('Instruction not recognized', instBin, this.PC);
      }
  
      // Increment the PC register to go to the next instruction. Instructions
      // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
      // instruction byte tells you how many bytes follow the instruction byte
      // for any particular instruction.
      if (this.pcAdvance) this.PC += argNum + 1;
    }
  }
  
  module.exports = CPU;