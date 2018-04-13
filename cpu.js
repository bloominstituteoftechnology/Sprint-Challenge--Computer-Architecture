const ADD  = 0b10101000; // ADD R R
const CALL = 0b01001000; // CALL R
const CMP  = 0b10100000; // CMP R R
const HLT  = 0b00000001; // Halt CPU
const LDI  = 0b10011001; // LDI R,I(mmediate)
const MUL  = 0b10101010; // MUL R,R
const POP  = 0b01001100; // Pop R
const PRN  = 0b01000011; // Print numeric register
const PUSH = 0b01001101; // Push R
const RET  = 0b00001001; // Return
const JEQ  = 0b01010001;
const JNE  = 0b01010010;
const JMP  = 0b01010000;

let E_FLAG = 0;
let G_FLAG = 0;
let L_FLAG = 0;

class CPU {
    constructor(ram) {
        this.ram = ram;
        this.reg = new Array(8).fill(0);
        this.reg.PC = 0;
    }

    poke(address, value) {
        this.ram.write(address, value);
    }

    startClock() {
        this.clock = setInterval(() => {
            this.tick();
        }, 1);
    }

    stopClock() {
        clearInterval(this.clock);
    }

    alu(op, regA, regB) {
        switch (op) {
            case 'MUL':
                this.reg[0] = regA * regB;
                break;
        }
    }

    tick() {
        let IR = this.ram.read(this.reg.PC);
        let operandA = this.ram.read(this.reg.PC + 1);
        let operandB = this.ram.read(this.reg.PC + 2);
        let addLen = true;

        switch(IR){
            case LDI:      
                this.reg[operandA] = operandB;
                break; 

            case CMP:
                this.reg[operandA] === this.reg[operandB] ? E_FLAG = 1 : E_FLAG = 0;
                this.reg[operandA] < this.reg[operandB] ? L_FLAG = 1 : L_FLAG = 0;
                this.reg[operandA] > this.reg[operandB] ? G_FLAG = 1 : G_FLAG = 0;
                break;

            case JMP:
                this.reg.PC = this.reg[operandA];
                addLen = false;
                break;

            case JNE:
                if(E_FLAG === 0){
                   this.reg.PC = this.reg[operandA];
                   addLen = false;
                }
                break;  

            case JEQ:
                if(E_FLAG === 1){
                   this.reg.PC = this.reg[operandA];
                   addLen = false;
                }
                break;         

            case PRN:
                console.log(this.reg[operandA]);
                break;

            case HLT:
            this.stopClock();
            break;
            
            default:
            console.log(`error: unknown instruction at PC: ${this.reg.PC}. Return instruction: ${this.ram.read(this.reg.PC)}`);
            this.stopClock();
            break;
        }

        if(addLen){
            let instructionLen = (IR >>> 6) + 1;
            this.reg.PC += instructionLen; 
        }  
    }
}

module.exports = CPU;
