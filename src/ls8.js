const fs = require('fs');
const RAM = require('./ram');
const Keyboard = require('./keyboard');
const { CPU, HLT, LDI, MUL, PRN, PUSH, POP, CALL, RET, INT, IRET, JMP, ADD, SUB, ST, NOP, PRA, CMP, INC,
        DEC, JEQ, JNE, LD, DIV } = require('./cpu');

let ram = new RAM(256);
let cpu = new CPU(ram);
const keyboard = new Keyboard();
keyboard.ConnectToCPU(cpu);
// console.log('constants',HLT,LDI,MUL,PRN)
const iL = [
]
iL[HLT & 0x3F] = HLT;
iL[LDI & 0x3F] = LDI;
iL[MUL & 0x3F] = MUL;
iL[PRN & 0x3F] = PRN;
iL[PUSH & 0x3F] = PUSH;
iL[POP & 0x3F] = POP;
iL[CALL & 0x3F] = CALL;
iL[RET & 0x3F] = RET;
iL[INT & 0x3F] = INT;
iL[IRET & 0x3F] = IRET;
iL[JMP & 0x3F] = JMP;
iL[ADD & 0x3F] = ADD;
iL[SUB & 0x3F] = SUB;
iL[ST & 0x3F] = ST;
iL[NOP & 0x3F] = NOP;
iL[PRA & 0x3F] = PRA;
iL[CMP & 0x3F] = CMP;
iL[INC & 0x3F] = INC;
iL[DEC & 0x3F] = DEC;
iL[JEQ & 0x3F] = JEQ;
iL[JNE & 0x3F] = JNE;
iL[LD & 0x3F] = LD;
iL[DIV & 0x3F] = DIV;
iL[JNE & 0x3F] = JNE;

console.log(`iL[0]: ${iL[0]} NOP & ${NOP & 0x3F} NOP: ${NOP} `);
console.log(`iL[JNE & 0x3F]: ${iL[JNE & 0x3F].toString(2)} JNE & 0x3F ${(JNE & 0x3F).toString(2)} JNE: ${JNE.toString(2)} `);

// console.log(`iL: ${iL}  il[RET]: ${iL[0b00010000].toString(2)}  iL[LDI]: ${iL[4].toString(2)}`)
/**
 * Process a loaded file
 **/
function processFile(content, cpu, onComplete) {
    let curAddr = 0;
    const lines = content.split('\n');

    let state = 0
    for (let line of lines) {
        const commentIndex = line.indexOf('#');
        if (commentIndex >= 0) {
            if (/^\# TEXT/.test(line)) {
                state = Math.max();
                // console.log(`ls8 text ${line}`);
            }
            line = line.substring(0, commentIndex);
        }
        line = line.trim()
        if (line.length < 8) continue
        let ri = parseInt(line.substring(0, 8), 2);
        let i = ri;
        // console.log(`raw i: ${i.toString(2)} state: ${state} `);
        if (!state) {
            i = iL[ri];
            state = ((i & 0xC0) >> 6) + 1;
            // console.log(i & 0xC0);
            if (i === undefined)
                console.log(`command : state: ${state}  line: ${line} iL i: ${i !== undefined ? i.toString(2) : 'not working'}
                ri: ${ri !== undefined ? ri.toString(2) : 'not working'}  iL[0]: ${iL[0]}`);
        }
        if (i === undefined) {
            console.log(`state: ${state} curAddr: ${curAddr}  line: ${line.substring(0, 8)} 
            iL i: ${i !== undefined ? i.toString(2) : 'not working'}
            ri: ${ri !== undefined ? ri.toString(2) : 'not working'}  `);
        }
        cpu.poke(curAddr, i)
        curAddr++;
        state--;
    }
    cpu.setTOP(curAddr)
    onComplete(cpu);
}

/**
 * Load the instructions into the CPU from stdin
 */
function loadFileFromStdin(cpu, onComplete) {
    let content = '';

    // Read everything from standard input, stolen from:
    // https://stackoverflow.com/questions/13410960/how-to-read-an-entire-text-stream-in-node-js
    process.stdin.resume();
    process.stdin.on('data', function (buf) { content += buf.toString(); });
    process.stdin.on('end', () => { processFile(content, cpu, onComplete); });
}

/**
 * Load the instructions into the CPU from a file
 */
function loadFile(filename, cpu, onComplete) {
    const content = fs.readFileSync(filename, 'utf-8');
    processFile(content, cpu, onComplete);
}

/**
 * On File Loaded
 * 
 * CPU is set up, start it running
 */
function onFileLoaded(cpu) {
    cpu.startClock();
    cpu.startINTClock();
}

/**
 * Main
 */




// Get remaining command line arguments
const argv = process.argv.slice(2);
// console.log(`argv: ${argv}`);
// Check arguments
if (argv.length === 0) {
    // Read from stdin
    loadFileFromStdin(cpu, onFileLoaded);
} else if (argv.length == 1) {
    //  console.log(`loadFile ${argv[0]}`)
    // Read from file
    loadFile(argv[0], cpu, onFileLoaded);
} else {
    console.error('usage: ls8 [machinecodefile]');
    process.exit(1);
}
