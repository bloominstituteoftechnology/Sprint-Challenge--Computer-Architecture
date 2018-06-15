const RAM = require('./ram');
const CPU = require('./cpu');
const fs = require('fs');

/**
 * Load an LS8 program into memory
 *
 * TODO: load this from a file on disk instead of having it hardcoded
 */
function loadMemory() {
    const program = [];
    const filename = process.argv[2];
    const file = fs.readFileSync(filename, 'utf8');
    file.split(/[\r\n]+/g).forEach(x => {
        const op = x.split(' ')[0];
        if(op.length === 8) program.push(op);
    });

    //instructor solve for unhardcode
    // const content = fs.readFileSync(filename, 'utf-8');
    // console.log(content);

    // const lines = content.trim().split(/[\r\n]+/g);
    
    // program = [];
    
    // for (let line of lines) {
    //     console.log(line);
    //     const val = parseInt(line, 2);

    //     if (isNaN(val)) {
    //         continue;
    //     }

    //     program.push(val);
    // }
    //NOTE must remove the parseInt from below as well to be this: 
//     for (let i = 0; i < program.length; i++) {
//         cpu.poke(i, program[i]);
//     }
// }

    // Hardcoded program to print the number 8 on the console

    // const program8 = [ // print8.ls8
    //     "10011001", // LDI R0,8  Store 8 into R0
    //     "00000000",
    //     "00001000",
    //     "01000011", // PRN R0    Print the value in R0
    //     "00000000",
    //     "00000001"  // HLT       Halt and quit
    // ];

// ----------------------------------------------------------------------------

    // Hardcoded program to print the number 72 on the console

    // const program = [ // mult.ls8
        // "10011001", // LDI R0,8  Load R0 with value 8
        // "00000000",
        // "00001000",
        // "10011001", // LDI R1,9  Load R1 with value 9
        // "00000001",
        // "00001001",
        // "10101010", // MUL R0,R1 Multiply R0*R1, storing result in R0
        // "00000000",
        // "00000001",
        // "01000011", // PRN R0    Print value in R0
        // "00000000",
        // "00000001" // HLT       Halt
    // ];

// ----------------------------------------------------------------------------    
    //Update this if going with instructor unhardcode above
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

// TODO: get name of ls8 file to load from command line
loadMemory(cpu);

cpu.startClock();