/**
 * LS-8 v2.0 emulator skeleton code
 */

const fs = require('fs');
const readline = require('readline');

// Instructions
// high to bits have arg count 0-3
const HLT = 0b00011011; // Halt CPU
const LDI = 0b10000100;
const MUL = 0b10000101;
const PRN = 0b01000110;
const CALL = 0b01001111;
const RET = 0b00010000;
const PUSH = 0b01001010;
const POP = 0b01001011;
const INT = 0b00011001;
const IRET = 0b00011010;
const JMP = 0b01010001;
const ADD = 0b10001100;
const SUB = 0b10001101;
const ST = 0b10001001;
const NOP = 0b00000000;
const PRA = 0b01000111;
const CMP = 0b10010110;
const INC = 0b01010111;
const DEC = 0b01011000;
const JEQ = 0b01010011;
const JNE = 0b01010100;
const LD = 0b10010010;
const DIV = 0b10001110;

const ops = {
    "ADD": { type: 2, code: '00001100' },
    "CALL": { type: 1, code: '00001111' },
    "CMP": { type: 2, code: '00010110' },
    "DEC": { type: 1, code: '00011000' },
    "DIV": { type: 2, code: '0001110' },
    "HLT": { type: 0, code: '00011011' },
    "INC": { type: 1, code: '00010111' },
    "INT": { type: 1, code: '00011001' },
    "IRET": { type: 0, code: '00011010' },
    "JEQ": { type: 1, code: '00010011' },
    "JMP": { type: 1, code: '00010001' },
    "JNE": { type: 1, code: '00010100' },
    "LD": { type: 2, code: '00010010' },
    "LDI": { type: 8, code: '00000100' },
    "MUL": { type: 2, code: '00000101' },
    "NOP": { type: 0, code: '00000000' },
    "POP": { type: 1, code: '00001011' },
    "PRA": { type: 1, code: '00000111' },
    "PRN": { type: 1, code: '0000110' },
    "PUSH": { type: 1, code: '00001010' },
    "RET": { type: 0, code: '00010000' },
    "ST": { type: 2, code: '00001001' },
    "SUB": { type: 2, code: '00001101' },
};

const IM = 5;
const IS = 6;
const SP = 7;

const TOS = 0xF6;
const ALLBITS = 0xFF;
const INT0 = 0xF8;
// const Readline = new readline();
const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: true
});
const kl = 'keystroke.logger.txt';
var KeystrokeOutput = null;
fs.unlink(kl, (err) => KeystrokeOutput = fs.openSync(kl, 'w'));



console.log(`rl.output.rows keys: ${Object.keys(rl.output.rows)} rl.output keys: ${Object.keys(rl.output)}`);
/**
 * Class for simulating a simple Computer (CPU & memory)
 */
class CPU {

    /**
     * Initialize the CPU
     */
    constructor(ram) {
        this.ram = ram;

        this.reg = new Array(8).fill(0); // General-purpose registers
        // Special-purpose registers
        this.inr = {}
        this.inr.PC = 0; // Program Counter
        this.inr.IR = 0; // Instruction Register
        this.inr.MAR = 0; // Memory Address Register, holds the memory address we're reading or writing
        this.inr.MDR = 0; // Memory Data Register, holds the value to write or the value just read
        this.reg[SP] = TOS;
        this.flags = {
            equal: false,
            overflow: false,
            divideBy0: false
        }
        this.int0Counter = 0;
        this.testingInt = false;
        this.halted = false;
        // for debugging only
        this.pushPopCount = 0;
        this.waitPromises = []
        this.tc = 0;
        this.opCodes = []
        Object.keys(ops).forEach(key => {
            this.opCodes[Number('0b' + ops[key].code)] = key;
        });
        this.testingINT = true;
        // end for debugging only
        this.setupBranchTable();
    }
    /**
     * Raise an interrupt
     */
    raiseInterrupt(n) {
        this.reg[IS] |= n;
        // console.log(`raiseInterrupt IS: ${this.reg[IS].toString(2)} IM: ${this.reg[IM].toString(2)} `)
        this.INT();
    }
    /**
     * Store value in memory address, useful for program loading
     */
    poke(address, value) {
        this.ram.write(address, value);
    }

    read() {
        this.inr.MDR = this.ram.read(this.inr.MAR);
    }
    readPcInc() {
        this.inr.MAR = this.inr.PC;
        this.read();
        this.inr.PC++;
        return this.inr.MDR;
    }
	/**
	 * Sets up the branch table
	 */
    setupBranchTable() {
        let bt = {};

        bt[HLT] = this.HLT;
        // !!! IMPLEMENT ME
        bt[LDI] = this.LDI;
        bt[MUL] = this.MUL;
        bt[PRN] = this.PRN;
        bt[CALL] = this.CALL;
        bt[RET] = this.RET;
        bt[PUSH] = this.PUSH;
        bt[POP] = this.POP;
        bt[JMP] = this.JMP;
        bt[IRET] = this.IRET;
        bt[ST] = this.ST;
        bt[NOP] = this.NOP;
        bt[PRA] = this.PRA;
        bt[CMP] = this.CMP;
        bt[INC] = this.INC;
        bt[DEC] = this.DEC;
        bt[ADD] = this.ADD;
        bt[SUB] = this.SUB;
        bt[JEQ] = this.JEQ;
        bt[JNE] = this.JNE;
        bt[LD] = this.LD;
        bt[DIV] = this.DIV;

        this.branchTable = bt;
    }
    setTOP(i) {
        this.TOP = i;
        //  console.log(`TOP: ${this.TOP}`);
    }
    randomIND() {
        return Math.random(Math.random() * 7);
    }

    /**
     * Store value in memory address, useful for program loading
     */
    poke(address, value) {
        this.ram.write(address, value);
    }
    // timer interupt clock
    startINTClock() {
        // console.log('started INT Clock')
        const _this = this;

        this.INTclock = setInterval(() => {
            if (_this.reg[IM]) {
                // console.log(`calling INT0 with mask ${_this.reg[IM].toString(2)} stack size: ${TOS - _this.reg[SP]}  
                //     Push/Pop count: ${_this.pushPopCount} `)
                _this.raiseInterrupt(1);

            }
        }, 400);
    }
    stopINTClock() {
        clearInterval(this.INTclock);
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
     * Stops the clock
     */
    stopClock() {
        clearInterval(this.clock);
    }

    /**
     * ALU functionality
     * 
     * op can be: ADD SUB MUL DIV INC DEC CMP
     */
    alu(op, regA, regB) {
        // console.log(`op: ${op} regA: ${regA} regB: ${regB}`)
        switch (op) {
            case 'MUL':
                this.reg[regA] = this.reg[regA] * this.reg[regB];
                break;
            case 'LDI':
                this.reg[regA] = regB;
                break;
            case 'ADD':
                this.reg[regA] = this.reg[regA] + this.reg[regB];
                break;
            case 'SUB':
                this.reg[regA] = this.reg[regA] - this.reg[regB];
                break;
            case 'INC':
                this.reg[regA]++;
                break;
            case 'DEC':
                this.reg[regA]--;
                break;
            case 'DIV':
                if (this.reg[regB] === 0) {
                    this.flags.divideBy0 = true;
                    this.reg[regA] = NaN;
                } else {
                    this.flags.divideBy0 = false;
                    this.reg[regA] = this.reg[regA] / this.reg[regB];
                }
                break;
            case 'CMP':
                this.flags.equal = (this.reg[regA] === this.reg[regB]);
                break;
        }
    }

    /**{
     * Advances the CPU one cycle
     */
    tick() {
        // Load the instruction register from the current PC

        this.inr.MAR = this.inr.PC;
        this.read();
        this.inr.IR = this.inr.MDR;
        // Debugging output0000110
        // ops.filter(o => o.)
        // console.log(`opCode: ${this.inr.IR !== undefined ? this.opCodes[this.inr.IR & 0x3F] : 'null'}  
        //  PC  ${this.inr.PC}:  IR:  ${this.inr.IR !== undefined ? this.inr.IR.toString(2) : 'error'}`);
        this.inr.PC++;
        const argCount = (this.inr.IR & 0b11000000) >> 6;
        let arg1, arg2, arg3;
        if (argCount >= 1) arg1 = this.readPcInc();
        if (argCount >= 2) arg2 = this.readPcInc();
        if (argCount === 3) arg3 = this.readPcInc();
        // console.log(`args ${argCount}: ${argCount >= 1 ? arg1 : ''}  ${argCount >= 2 ? arg2 : ''} `)        

        // Based on the value in the Instruction Register, jump to the
        // appropriate hander in the branchTable
        const handler = this.branchTable[this.inr.IR];
        if (handler === undefined) {
            console.log(`undefined handler HLT  tc: ${this.tc}  PC: ${this.inr.PC} IR: ${this.inr.IR ? this.inr.IR.toString(2) : 'null'}`);
            this.HLT.call(this)
            return
        }


        // Check that the handler is defined, halt if not (invalid
        // instruction)

        // We need to use call() so we can set the "this" value inside
        // the handler (otherwise it will be undefined in the handler)

        handler.call(this, arg1, arg2, arg3);
        this.tc++;
    }
    int0() {
        this.int0Counter++;
        for (let i = 0; i < 7; i++) this.reg[i] = this.int0Counter + i;
        this.PRN(`reg[0]: ${reg[0]} reg[1]: ${reg[1]} reg[2]: ${reg[2]} reg[3]: ${reg[3]} reg[4]: ${reg[4]} reg[5]: ${reg[5]} reg[6]: ${reg[6]} reg[7]: ${reg[7]} `)
    }
    sleep() {
        const timer = ms => new Promise(res => setTimeout(res, ms));
        return timer;
    }
    // INSTRUCTION HANDLER CODE: // !!! IMPLEMENT ME
    /**
     * HLT
     */
    HLT() {
        this.halted = true;
        this.stopClock();
        const promiseSerial = funcs =>
            funcs.reduce((promise, func) =>
                promise.then(result => func(500).then(Array.prototype.concat.bind(result))),
                Promise.resolve([]));

        promiseSerial(this.waitPromises)
            .then(_ => {
                // console.log('stopping INT clock');
                this.stopINTClock();
                // console.log(`R5: ${this.reg[IM].toString(2)}  R6: ${this.reg[IS].toString(2)} `)
                /*
* R5 is reserved as the interrupt mask (IM)
* R6 is reserved as the interrupt status (IS)                
                */
            })
            .catch(console.error.bind(console));
        this.reg[0] = 0xFF;
        this.WAIT(0);
    }

    /**
     * LDI R,I
     */
    LDI(R, I) {
        this.alu('LDI', R, I)
        // !!! IMPLEMENT ME

    }

    /**
     * MUL R,L
     */
    MUL(regA, regB) {
        // !!! IMPLEMENT ME
        this.alu('MUL', regA, regB)
    }
    PUSH(reg) {
        this.pushPopCount++;
        // console.log(`PUSHing reg: ${reg}  value: ${this.reg[reg]} to ${this.reg[SP].toString(16)}`);
        if (this.reg[SP] < this.TOP + 1) throw "push at TOP";
        //  console.log(`before push ${this.reg[SP]}`)
        const v = (this.reg[SP])--;
        // console.log(`after push ${this.reg[SP]}`)
        this.ram.write(v, this.reg[reg]);
        //  console.log(this.ram.read(this.reg[SP]+1))   
        //  console.log(`SP: ${this.reg[SP]}`);
    }
    pop() {
        this.pushPopCount--;
        // console.log(`before pop ${this.reg[SP]}`)
        const v = ++this.reg[SP];
        // console.log(`after pop ${this.reg[SP]}`)
        return this.ram.read(v);
    }
    POP(reg) {
        this.reg[reg] = this.pop();
        // console.log(`POPing from ${this.reg[SP]}`);
    }
    CALL(reg) {
        // console.log(`Call to ${address}`);
        const temp = this.reg[3];
        this.reg[3] = this.inr.PC;
        this.PUSH(3);
        this.reg[3] = temp;
        this.inr.PC = this.reg[reg];
        // console.log(`calling ${this.ram.read(address).toString(2)} CALL length  ${((CALL & 0b11000000) >> 6)}`)
    }
    RET() {
        this.inr.PC = this.pop();
        // console.log(`RET to ${this.inr.PC}`);
    }
    JMP(reg) {
        this.inr.PC = this.reg[reg];
    }
    ADD(regA, regB) {
        this.alu('ADD', regA, regB);
    }
    SUB(regA, regB) {
        this.alu('SUB', regA, regB);
    }

    /**
     * PRN R
     */
    PRN(R) {
        // !!! IMPLEMENT ME
        console.log(`PNR==+++++++++++++++++> R: ${R}  ${this.reg[R]}  <=+++++++++++++++++=`)
        //console.log(this.reg[R]);
    }
    INT() {
        //  console.log(`INT IM: ${this.reg[IM].toString(2)}  IS: ${this.reg[IS].toString(2)} `);
        //if (this !== _this) console.log(`this delta ${_this - this}`)
        if (!this.reg[IM]) return;
        // Check if an interrupt happened
        const maskedInterrupts = this.reg[IS] & this.reg[IM];
        let bits = maskedInterrupts.toString(2).padStart(8, '0');
        // console.log(`bits: ${bits}`);
        for (let i = bits.length - 1; i >= 0; i--) {
            if (bits[i] != '0') {
                // console.log(`before IS: ${this.reg[IS].toString(2)}`);
                this.reg[IM] = 0;  // disable all interupts
                //  console.log('IM = 0');
                //  console.log(`found INT${7 - i}  bits[i]: ${bits[i]} i: ${i}  PC: ${this.inr.PC} `);
                let bitsA = bits.split('');
                bitsA[i] = '0'; // only turn off this interupt
                bits = bitsA.join('');
                this.reg[IS] = Number.parseInt(bits, 2);
                // console.log(`i: ${i} bits: ${bits} after IS: ${this.reg[IS].toString(2)}`);
                const _SP = this.reg[SP];
                // console.log(`_SP: ${_SP}`);
                const temp = this.reg[3];
                this.reg[3] = this.inr.PC;
                this.PUSH(3);
                this.reg[3] = temp;

                for (let j = 0; j < 7; j++) {
                    this.PUSH(j)
                }
                this.reg[0] = _SP;
                this.PUSH(0);
                this.inr.PC = this.ram.read(INT0 + 7 - i);
                // console.log(`i: ${i} INT PC: ${this.inr.PC}`);
                this.stopClock(); // make sure it's stopped
                this.startClock();
                break;
            }
        }
        // console.log(`INT exit PC: ${this.inr.PC}`);
    }

    IRET() {
        // console.log(`iRET SP: ${this.reg[SP]}`)
        // const badPop = this.pop();
        //console.log(`bad pop: ${badPop} SP: ${this.reg[SP]}`);
        const r7 = this.pop();
        for (let i = 6; i >= 0; i--) {
            this.reg[i] = this.pop()
            // console.log(`restored ${this.reg[i]} to reg[${i}]`);
        }
        this.inr.PC = this.pop();
        const toPop = this.reg[SP] - r7;
        if (toPop) console.log(`IRET delta: ${toPop}`)
        // for (let i = 0;i<toPop; i++) {
        //     console.log(`IRET popping`)
        //     this.pop();
        // }
        if (r7 === undefined) {
            console.log(`undefined r7`)
        }
        else
            this.reg[SP] = r7;

        // console.log(`IRET to ${this.inr.PC}`);
        this.reg[IM] = ALLBITS; // enable all interupts
        // console.log(`set IM: ${this.reg[IM].toString(2)}`);
        if (this.halted) this.stopClock();
    }
    ST(T, S) {
        this.ram.write(this.reg[T], this.reg[S]);
        //  console.log(`ST source: ${this.reg[S]}  target: ${this.reg[T].toString(16)}`);
        //  console.log(`INT0:  ${this.ram.read(INT0)}  value[2] ${this.ram.read(this.ram.read(INT0) + 2).toString(2)} `);

    }
    NOP() {
        if (this.testingINT) this.waitPromises.push(this.sleep());
    }
    PRA(reg) {
        const key = this.reg[reg];
        if (reg !== 3)
            if (key < 0x20 || key >= 0x7F) {
                fs.writeSync(KeystrokeOutput, `key: ${key.toString(16)}\n`);
            }
            else
                fs.writeSync(KeystrokeOutput, String.fromCharCode(key));
        switch (key) {
            case 0x0D: // carriage return
                //console.log();
                process.stdout.write(String.fromCharCode(0xA)); // line feed
                break;
            case 0x1b: // delete
                fs.writeSync(KeystrokeOutput, 'delete\n');
                readline.moveCursor(readline.output, 200, 0);
                // process.stdout.write(' ')
                // readline.moveCursor(readline.output, -1, 0);
                break;
            case 0x7f: // backspace
                fs.writeSync(KeystrokeOutput, 'backspace\n');
                readline.moveCursor(readline.output, 2000, 0);
                break;
            default:
                // if (reg === 3)
                process.stdout.write(String.fromCharCode(key));
                break;
        }
    }
    CMP(regA, regB) {
        this.alu('CMP', regA, regB);
    }
    INC(reg) {
        this.alu('INC', reg)
    }
    DEC(regA, regB) {
        this.alu('DEC', regA, regB)
    }
    JEQ(reg) {
        if (this.flags.equal) this.JMP(reg);
    }
    JNE(reg) {
        if (!this.flags.equal) this.JMP(reg);
    }
    LD(regA, regB) {
        this.reg[regA] = this.ram.read(this.reg[regB]);
    }
    DIV(regA, regB) {
        this.alu('DIV', regA, regB);
    }
    wait(ms) {
        return new Promise((r, j) => setTimeout(r, ms));
    }
    WAIT(reg) {
        console.log('start WAIT');
        const ms = (this.reg[reg] === 0xFF) ? -1 : this.reg[reg];
        this.wait(ms);
        console.log('exit WAIT');
    }
}

module.exports = {
    CPU,
    HLT,
    MUL,
    LDI,
    PRN,
    PUSH,
    POP,
    CALL,
    RET,
    ADD,
    SUB,
    JMP,
    IRET,
    ST,
    NOP,
    PRA,
    CMP,
    INC,
    DEC,
    JEQ,
    JNE,
    LD,
    DIV,
    KeystrokeOutput
}
