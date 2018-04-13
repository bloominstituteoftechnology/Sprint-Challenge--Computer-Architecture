/**
 * LS-8 v2.0 emulator skeleton code
 */

class CPU {
    constructor(ram) {
        this.ram = ram;
        this.reg = new Array(8).fill(0); // General-purpose registers R0-R7
        this.SP = 0xf4;
        // Special-purpose registers
        this.reg.PC = 0; // Program Counter
    }

    poke(address, value) {
        this.ram.write(address, value);
    }

    startClock() {
        this.clock = setInterval(() => {
            this.tick();
        }, 1); // 1 ms delay == 1 KHz clock == 0.000001 GHz
    }

    stopClock() {
        clearInterval(this.clock);
    }

    alu(op, regA, regB) {
        switch (op) {
            case 'MUL':
                const indexA = parseInt(regA, 2);
                const indexB = parseInt(regB, 2);
                this.reg[indexA] = this.reg[indexA] * this.reg[indexB];
                break;
        }
    }

    tick() {
        this.reg[7] = this.SP;
        const IR = this.ram.read(this.reg.PC); // binary number

        const operandA = this.ram.read(this.reg.PC + 1);
        const operandB = this.ram.read(this.reg.PC + 2);

        switch (IR) {
            case '10011001':
                this.LDI(operandA, operandB);
                break;
            case '01000011':
                this.PRN(operandA);
                break;
            case '00000001':
                this.HLT();
                break;
            case '10101010':
                this.alu('MUL', operandA, operandB);
                break;
            case '01001100':
                this.POP(operandA);
                break;
            case '01001101':
                this.PUSH(operandA);
                break;
            case '01001000':
                this.CALL(operandA);
                break;
            case '00001001':
                this.RET();
                break;
            case '10101000':
                this.ADD(operandA, operandB);
                break;
            default:
        }

        this.reg.PC = this.reg.PC + parseInt(IR.slice(0, 2), 2) + 1;
    }

    PRN(register) {
        const index = parseInt(register, 2);
        console.log('our answer: ', parseInt(this.reg[index], 2));
    }

    LDI(register, immediate) {
        const index = parseInt(register, 2);
        this.reg[index] = immediate;
    }

    HLT() { this.stopClock(); }

    PUSH(register) {
        const index = parseInt(register, 2);
        this.SP--;
        this.ram[this.SP] = this.reg[index];
    }

    _push(value) {
        this.SP--;
        this.ram[this.SP] = value;
    }

    POP(register) {
        const index = parseInt(register, 2);
        this.reg[index] = this.ram[this.SP];
        this.SP++;
    }

    _pop() {
        const value = this.ram[this.SP];
        this.SP++;
        return value;
    }

    CALL(register) {
        this._push(this.reg.PC);
        const index = parseInt(register, 2);
        this.reg.PC = parseInt(this.reg[index], 2) - 2;
    }

    RET() { this.reg.PC = this._pop() + 1; }

    ADD(registerA, registerB) {
        const indexA = parseInt(registerA, 2);
        const indexB = parseInt(registerB, 2);
        this.reg[indexA] = (
            parseInt(this.reg[indexA], 2) + parseInt(this.reg[indexB], 2)
        ).toString(2);
    }
}

module.exports = CPU;
