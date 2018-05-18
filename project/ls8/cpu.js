/**
 * LS-8 v2.0 emulator skeleton code
 */

/**
 * Class for simulating a simple Computer (CPU & memory)
 */

const ADD = 0b10101000;
const AND = 0b10110011;
const CALL = 0b001001000;
const CMP = 0b10100000;
const DEC = 0b001111001;
const HLT = 0b00000001;
const INC = 0b001111000;
const INT = 0b001001010;
const IRET = 0b000001011;
const JEQ = 0b001010001;
const JGT = 0b001010100;
const JLT = 0b001010011;
const JMP = 0b001010000;
const JNE = 0b001010010;
const LD = 0b10011000;
const LDI = 0b10011001;
const MOD = 0b10101100;
const MUL = 0b10101010;
const NOP = 0b00000000;
const NOT = 0b001110000;
const OR = 0b10110001;
const POP = 0b001001100;
const PRA = 0b001000010; 
const PRN = 0b01000011; 
const PUSH = 0b001001101; 
const RET = 0b00001001;
const ST = 0b10011010;
const SUB = 0b10101001;
const XOR = 0b10110010; 

class CPU {

    /**
     * Initialize the CPU
     */
    constructor(ram) {
        this.ram = ram;

        this.reg = new Array(8).fill(0); // General-purpose registers R0-R7
        this.reg[7] = 0xF4;

        // Special-purpose registers
        this.PC = 0; // Program Counter
        this.MAR = 0;
        this.MDR = 0;
        this.FL = 0;
        this.IM = this.reg[5];
        this.IS = this.reg[6];
        this.SP = this.reg[7];
        this.JMP = 0;
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
      let num1 = this.reg[regA];
      let num2 = this.reg[regB];

        switch (op) {
            case 'ADD':
              this.reg[regA] = num1 + num2;
              break;
            case 'SUB':
              this.reg[regA] = num1 - num2;
              break;
            case 'MUL':
              this.reg[regA] = num1 * num2;
              break;
            case 'DIV':
              this.reg[regA] = num1 / num2;
              break;
            case 'INC':
              this.reg[regA] = num1++;
              break;
            case 'DEC':
              this.reg[regA] = num1--;
              break;
            default:
              this.hlt();
        }
    }

    /**
     * Advances the CPU one cycle
     */
    tick() {
        // console.log('Flag: ', this.FL);
        // Load the instruction register (IR--can just be a local variable here)
        // from the memory address pointed to by the PC. (I.e. the PC holds the
        // index into memory of the instruction that's about to be executed
        // right now.)

        let IR = this.ram.read(this.PC);
        
        // Debugging output
        // console.log(`${this.PC}: ${IR.toString(2)}`);


        // Get the two bytes in memory _after_ the PC in case the instruction
        // needs them.

        let operandA = this.ram.read(this.PC + 1);
        let operandB = this.ram.read(this.PC + 2);
        // console.log('OperandA: ', this.reg[operandA]);
        // Execute the instruction. Perform the actions for the instruction as
        // outlined in the LS-8 spec.
        switch (IR) {
          case LDI:
            this.reg[operandA] = operandB;
            break;
          case PRN:
            console.log(this.reg[operandA]);
            break;
          case HLT:
            this.stopClock();
            break;
          case ADD:
            this.alu("ADD", operandA, operandB);
            break;
          case MUL:
            this.alu("MUL", operandA, operandB);
            break;
          case CALL:
            this.SP--;
            this.poke(this.SP, this.PC + 2);
            this.PC = this.reg[operandA];
            this.JMP = 1;
            break;
          case POP:
            this.reg[operandA] = this.ram.read(this.SP);
            this.SP++;
            break;
          case PUSH:
            this.SP--;
            this.poke(this.SP, this.reg[operandA]);
            break;
          case RET:
            this.PC = this.ram.read(this.SP);
            this.SP++;
            this.JMP = 1;
            break;
          case CMP:
            this.reg[operandA] === this.reg[operandB]
            ? this.FL = 0b000 | 0b001
              : this.reg[operandA] > this.reg[operandB]
              ? this.FL = 0b000 | 0b010
              : this.FL = 0b000 | 0b100;
            break;
          case JMP:
            this.PC = this.reg[operandA];
            this.JMP = 1;
            break;
          case JEQ:
            if (this.FL === 0b001) {
              this.FL = this.FL & 0b000;
              this.PC = this.reg[operandA];
              this.JMP = 1;
            }
            break;
          case JGT:
            if (this.FL === 0b010) {
              this.FL = this.FL & 0b000;
              this.PC = this.reg[operandA];
              this.JMP = 1;
            }
            break;
          case JLT:
            if (this.FL === 0b100) {
              this.FL = this.FL & 0b000;
              this.PC = this.reg[operandA];
              this.JMP = 1;
            }
          case JNE:
            if (this.FL === 0b100 || this.FL === 0b010) {
              this.FL = this.FL & 0b000;
              this.PC = this.reg[operandA];
              this.JMP = 1;
            }
            break;
          default:
            this.stopClock();
        }

        // Increment the PC register to go to the next instruction. Instructions
        // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
        // instruction byte tells you how many bytes follow the instruction byte
        // for any particular instruction.

        if (this.JMP !== 1) {
          this.PC += (IR >> 6) + 1;
        }
        
        this.JMP = 0;
    }
}

module.exports = CPU;
