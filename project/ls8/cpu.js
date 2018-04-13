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

        this.reg = new Array(8).fill(0); // General-purpose registers R0-R7 (8 bit)
        this.reg.IR = null;
        this.equal = 0;
        this.lessthan = 0;
        this.greaterthan = 0;
        // Special-purpose registers
        this.reg.PC = 0; // Program Counter
    }

    /**
     * Store value in memory address, useful for program loading
     */
    poke(address, value) {
        this.ram.write(address, value);
        console.log(`address: ${address}, value: ${value}`)
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
            return (regA * regB);
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

        this.reg.IR = this.ram.read(this.reg.PC); // grab byte 1

        // !!! IMPLEMENT ME

        // Debugging output
        console.log(`this.reg.IR ${this.reg.IR}`);
        console.log(`this.reg.PC: ${this.reg.PC} || this.reg.IR: ${this.reg.IR.toString(2)}`);

        // Get the two bytes in memory _after_ the PC in case the instruction
        // needs them.

        let byte1 = this.ram.read(this.reg.PC + 1); // grab byte 2
        let byte2 = this.ram.read(this.reg.PC + 2); // grab byte 3

        // !!! IMPLEMENT ME

        // Execute the instruction. Perform the actions for the instruction as
        // outlined in the LS-8 spec.
        switch (this.reg.IR) {
            case 170: // MUL

              console.log('MUL ran: multiply and store');
              console.log(`Multiplied ${this.reg[byte1]} by ${this.reg[byte2]}`);
              this.reg[byte1] = this.alu('MUL', this.reg[byte1], this.reg[byte2]);
              console.log(`Register: ${this.reg}`);
            break;
            case 160: // CMP
              console.log('CMP ran');

              if (this.reg[byte1] === this.reg[byte2]) {
                this.equal = 1;
                console.log(this.equal);
              } else if (this.reg[byte1] < this.reg[byte2]) {
                this.lessthan = 1;
                console.log(this.lessthan);
              } else if (this.reg[byte1] > this.reg[byte2]) {
                this.greaterthan = 1;
                console.log(this.greaterthan);
              }
            break;
            case 153: // LDI
              console.log('LDI ran: Store');
              console.log(`byte1: ${byte1}, byte2: ${byte2}`);
              this.reg[byte1] = byte2;
              console.log(this.reg);
              break;
            case 80: // JMP
              console.log('JMP ran');
              this.reg.PC = this.reg[byte1];
              break;
            case 81:
              console.log('JEQ ran');
              if (this.equal === true) {
                this.reg.PC = this.reg[byte1];
              }
              break;
            case 82: // JEQ
              if (this.equal === false) {
                this.reg.PC = this.reg[byte1];
              }
              break;
            case 67: // PRN
              console.log('PRN ran: Print');
              console.log(`Prints the value in ${byte1}: ${this.reg[byte1]}`)
              break;
            case 1: // HLT
              console.log('HLT ran: Halt and Quit');
              //this.HLT();
              this.stopClock();
              break;
        }
        // !!! IMPLEMENT ME

        // Increment the PC register to go to the next instruction. Instructions
        // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
        // instruction byte tells you how many bytes follow the instruction byte
        // for any particular instruction.

        // !!! IMPLEMENT ME
        // this.reg.PC += parseInt(this.reg.IR >> 6, 2);
        if (this.reg.IR.toString(2).length === 8) {
          if (this.reg.IR.toString(2).charAt(0) == 1) {
            this.reg.PC += 3;
          } else if (this.reg.IR.toString(2).charAt(1) == 1) {
            this.reg.PC += 2;
          }
          console.log('-------------------------------');
        } else if (this.reg.IR.toString(2).length === 7) {
          if (this.reg.IR.toString(2).charAt(0) == 1) {
            this.reg.PC += 2;
          }
          console.log('-------------------------------');
        } else {
          console.log('-------------------------------');
          this.reg.PC++;
        }
    }
}

module.exports = CPU;
