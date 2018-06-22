/**
 * LS-8 v2.0 emulator skeleton code
 */

const LDI = 0b10011001;
const PRN = 0b01000011;
const HLT = 0b00000001;
const MUL = 0b10101010;
const PUSH = 0b01001101;
const POP = 0b01001100;
const CALL = 0b01001000;
const RET = 0b00001001;
const CMP = 0b00001001;
const JMP = 0b01010000;
const JEQ = 0b01010001;
const JNE = 0b10100010;
const FLAG = 0;


let SP = 0x07;
// let IS = 0x06;
// let IM = 0x05;

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
       this.reg[SP] = 0xf4 // Point register 7 to register 4 which is empty

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


   stopClock() {
       clearInterval(this.clock);
   };
   
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

       // !!! IMPLEMENT ME
       const IR = this.ram.read(this.PC);

   
       // Get the two bytes in memory _after_ the PC in case the instruction
       // needs them.
       const operandA = this.ram.read(this.PC + 1); 
       const operandB = this.ram.read(this.PC + 2); 

   
       switch(IR) {
           case LDI:
               this.reg[operandA] = operandB;
               this.PC += 3;
               break;

           case MUL:
               this.reg = this.reg[0] *  this.reg[1];
               this.PC += 3;
               break;
           
           case PRN:
               console.log(this.reg[operandA]);
               this.PC += 2;
               break;

           case HLT:
               this.stopClock();
               this.PC += 1;
               break;

           case PUSH:
               this.pushValue(this.reg[operandA]);
               this.PC += 2;
               break;
           
           case POP:
               this.popValue(this.reg[operandA]);
               this.PC += 2;
               break;

           case CALL:
               this.pushValue(this.PC + 2);
               this.PC = this.reg[operandA];
               break;

           case RET:
               this.PC = this.popValue();
               break;

            case CMP:
                if (this.reg[operandA] === this.reg[operandB]) {
                    this.FLAG = 1;
                    this.PC += 3;
                } else {
                    this.FLAG = 0;
                    this.PC += 3;
                }
                break;

            case JMP:
                this.PC = this.reg[operandA];
                break;
            
            case JEQ: 
                if (this.FLAG === 1) {
                    this.PC = this.reg[operandA];
                } else {
                    this.PC += 2;
                } 
                break;
            
            case JNE:
                if (this.FLAG === 0) {
                    this.PC = this.reg[operandA];
                } else {
                    this.PC += 2;
                }
           default: 
               break;
       };
       
   }
   pushValue(operandA) {
       this.reg[SP] --;
       return this.ram.write(this.reg[SP], operandA)
       };
   
   popValue() {
       this.reg[SP] = this.ram.read(this.reg[SP])
       return this.reg[SP] ++;
   }
}

module.exports = CPU;
