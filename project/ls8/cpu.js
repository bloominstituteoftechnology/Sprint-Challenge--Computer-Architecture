const IM = 0x05;
const IS = 0x06;
const SP = 0x07;

//intializing flags
let equalFlag;
let greaterThanFlag;
let lessThanFlag;

const LDI = 0b10011001;
const HLT = 0b00000001;
const PRN = 0b01000011;
const MUL = 0b10101010;
const PUSH = 0b01001101;
const POP = 0b01001100;
//machine code for the methods to add today
const CMP = 0b10100000;
const JMP = 0b01010000;
const JEQ = 0b01010001;

class CPU {

    constructor(ram) {
        this.ram = ram;

        this.reg = new Array(8).fill(0);
        this.reg[SP] = 0xf4;
        this.reg.PC = 0;
        //variable to determine if the CPU should continue normal functioning and iterate the PC or stop
        this.continueOp = true;
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
        const firstVal = this.reg[regA];
        const secondVal = this.reg[regB];

        switch (op) {
            case 'ADD':
                return this.reg[regA] = firstVal + secondVal;
            case 'DEC':
                return this.reg[regA] = this.reg[regA] - 1;
            case 'DIV':
                if (firstVal || secondVal === 0){
                   return this.reg[regA] = 0;
                } else {
                    return this.reg[regA] = firstVal / secondVal;
                }
            case 'INC':
                return this.reg[regA] = this.reg[regA] + 1;
            case 'MOD':
                return this.reg[regA] = firstVal % secondVal;
            case 'MUL':
                return this.reg[regA] = firstVal * secondVal;
            case 'SUB':
                return this.reg[regA] = firstVal - secondVal;
            //case added today
            case 'CMP':
                equalFlag = (firstVal === secondVal);
                greaterThanFlag = (firstVal > secondVal);
                lessThanFlag = (firstVal < secondVal);
                return;
        }
    }

    tick() {
        const PC = this.reg.PC;
        const IR = this.ram.read(PC);

        console.log("\n");
        console.log(`${this.reg.PC}: ${IR.toString(2)}`);

        const operandA = this.ram.read(PC + 1);
        console.log("First potential operand: ", operandA.toString(2));
        const operandB = this.ram.read(PC + 2);
        console.log("Second potential operand: ", operandB.toString(2));

        //the method handler is set to the method stored in the branch table that IR is pointing to
        const methodHandler = this.opIndex[this.reg.IR];

        //checks if the spot in the branch table is undefined
        if (!methodHandler){
            console.log(`The current spot does not have a method stored`);
            this.continueOp = false;
            return this.stopClock();
        } else {
            console.log(`There was a method in the specified spot and it will be carried out`);
            return methodHandler(operandA, operandB);
        }

        const execute_LDI = () => {
            this.reg[operandA] = operandB;
        };
        const execute_HLT = () => {
            this.stopClock();
        };
        const execute_PRN = () => {
            console.log("Printline output: ", this.reg[operandA]);
        };
        const execute_MUL = () => {
            this.alu('MUL', operandA, operandB);
        }
        const execute_PUSH = () => {
            this.alu('DEC', SP);
            this.ram.write(this.reg[SP], this.reg[operandA]);
        };
        const execute_POP = () => {
            const val = this.ram.read(this.reg[SP]);
            this.alu('INC', SP);
            this.reg[operandA] = val;
            return;
        };

        //new methods added today
        const execute_CMP = () => {
            this.alu('CMP', operandA, operandB);
            return;
        };
        const execute_JMP = (val) => {
            this.reg.PC = this.reg[val];
            return this.continueOp = false;
        }
        const execute_JEQ = (val) => {
            if (equalFlag){
                this.reg.PC = this.reg[val];
                this.continueOp = false;
            } else {
                console.log(`There was an error executing the JEQ method`);
            }
        }

        const opIndex = [];
        opIndex[LDI] = execute_LDI;
        opIndex[HLT] = execute_HLT;
        opIndex[PRN] = execute_PRN;
        opIndex[MUL] = execute_MUL;
        opIndex[PUSH] = execute_PUSH;
        opIndex[POP] = execute_POP;
        //methods added to the branch table
        opIndex[CMP]= execute_CMP;
        opIndex[JMP] = execute_JMP;
        

        opIndex[IR]();

        //conditional that decides whether to increment the PC or not depending on the status of continueOp
        if (this.continueOp){
            const operandCount = IR >>> 6
            this.reg.PC++;
            this.reg.PC += operandCount;
        }
    }
}

module.exports = CPU;