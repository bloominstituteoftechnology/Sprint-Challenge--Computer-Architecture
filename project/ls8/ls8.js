const fs = require('fs');

const RAM = require('./ram');
const CPU = require('./cpu');

/**
 * Load an LS8 program into memory
 *
 *  load this from a file on disk instead of having it hardcoded
 * 
 */


// function loadMemory() {

    // Hardcoded program to print the number 8 on the console

    // const program = [ // print8.ls8
    //     "10011001", // LDI R0,8  Store 8 into R0
    //     "00000000",
    //     "00001000",
    //     "10011001", // LDI R1,9
    //     "00000001",
    //     "00001001",
    //     "10101010", // MUL R0,R1
    //     "00000000",
    //     "00000001",
    //     "01000011", // PRN R0    Print the value in R0
    //     "00000000",
    //     "00000001"  // HLT       Halt and quit
    // ];

    function loadMemory() {
        const program = [];
        const argv = process.argv;
        if (argv.length !== 3) {
          console.error('usage: node ls8 [filename]');
          process.exit(1);
        }

        const filename = argv[2];
        const filedata = fs.readFileSync(filename, 'utf8');
        const lines = filedata.trim().split(/[\r\n]+/g);

        for (let line of lines) {
            const cmt = line.indexOf('#');
            if (cmt !== -1) {
              line = line.substr(0, cmt);
            }
            if (line === '') {
              continue;
            }
            line = line.trim();
            program.push(line);
          }
      

    // Load the program into the CPU's memory a byte at a time
    for (let i = 0; i < program.length; i++) {
        cpu.poke(i, parseInt(program[i], 2));
    }
}

/**
 * Main
 */

let ram = new RAM(256);
let cpu = new CPU(ram);

// get name of ls8 file to load from command line

// argv = process.argv.slice(2);

// if(argv.length != 2){
//     console.error('usage: node [filename] [programname]');
//     process.exit(1);
// }

// const filename = argv[1];
// const programname = argv[2];

// const filedata = fs.readFileSync(filename, 'utf8');
// const progdata = fs.readFileSync(programname, 'utf8');

// const lines = filedata.trim().split(/[\r\n]+/g);

loadMemory(cpu);

cpu.startClock();