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
        let returnIndex;
        this.ram = ram;
        const LDI = 0b10011001;
        const HLT = 0b00000001;
        const PRN = 0b01000011;
        const POP = 0b01001100;
        const PUSH = 0b01001101;
        const CALL = 0b01001000;
        const RET = 0b00001001;
        const MUL = 0b10101010;

        this.branch = [];
        this.branch[LDI] = this.LDI.bind(this);
        this.branch[HLT] = this.HLT.bind(this);
        this.branch[PRN] = this.PRN.bind(this);
        this.branch[POP] = this.POP.bind(this);
        this.branch[PUSH] = this.PUSH.bind(this);
        this.branch[CALL] = this.CALL.bind(this);
        this.branch[RET] = this.RET.bind(this);


        this.reg = new Array(8).fill(0); // General-purpose registers R0-R7
        
        // Special-purpose registers
        this.PC = 0; // Program Counter
        this.SP = 0xF4;
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
              this.ram.write(regA,(this.ram.read(regA) * this.ram.read(regB)));
              break;
            case 'ADD':
              this.ram.write(regA,(this.ram.read(regA)+this.ram.read(regB)));
              break;
        }
    }

    POP(register){
      this.ram.write(register,this.ram.read(this.SP++));
    }

    PUSH(register){
      this.ram.write(--this.SP,this.ram.read(register));
    }

    HLT(){
      this.stopClock();
    }

    LDI(register,immediate){
      this.ram.write(register,immediate);
    }

    PRN(register){
      console.log(this.ram.read(register));
    }

    CALL(register){
      this.returnIndex = this.PC+1;
      this.PUSH(this.PC+1);
      this.PC = this.ram.read(register) - 1;
    }

    RET(){
      this.PC = this.returnIndex - 1;
      this.POP(this.returnIndex);
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

        let IR = this.ram.read(this.PC);


        // Debugging output
        //console.log(`${this.PC}: ${IR.toString(2)}`);

        // Get the two bytes in memory _after_ the PC in case the instruction
        // needs them.

        // !!! IMPLEMENT ME
        let operandA = null;
        let operandB = null;
        let argCount = IR >> 6
        if(argCount === 2){
          operandA = this.ram.read(++this.PC);
          operandB = this.ram.read(++this.PC);
        }
        else if(argCount === 1){
          operandA = this.ram.read(++this.PC);
        }

        // Execute the instruction. Perform the actions for the instruction as
        // outlined in the LS-8 spec.

        // !!! IMPLEMENT ME
        switch(IR){
          case 0b10101010:
            this.alu('MUL',operandA,operandB);
            break;
          case 0b10101000:
            this.alu('ADD',operandA,operandB);
            break;
          default:
            this.branch[IR](operandA,operandB);
        }

        // Increment the PC register to go to the next instruction. Instructions
        // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
        // instruction byte tells you how many bytes follow the instruction byte
        // for any particular instruction.
        
        // !!! IMPLEMENT ME
        this.PC++;
    }
}

module.exports = CPU;
