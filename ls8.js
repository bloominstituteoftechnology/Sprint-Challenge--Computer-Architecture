const fs = require('fs');
const RAM = require('./ram');
const CPU = require('./cpu');

if(process.argv[2] === undefined){
  console.log('error no instructions provided');
  process.exit(1);
}




/**
 * Load an LS8 program into memory
 *
 * TODO: load this from a file on disk instead of having it hardcoded
 */
function loadMemory() {
  let input = undefined;
    try {
      input = fs.readFileSync('./'+process.argv[2]).toString();
    }
    catch(err) {
      console.log('error invalid file');
      process.exit(1);
    }
    let lines = input.split('\n');
    let k = 0;
    
    for(let i = 0; i < lines.length; i++){
      if(lines[i][0] === '1' || lines[i][0] === '0'){
        cpu.poke(k++,parseInt(lines[i].slice(0,8),2));
      }
    }

    // Hardcoded program to print the number 8 on the console

/* mult.ls8
    const program = [
    '10011001', // LDI R0,8  Load R0 with value 8
    '00000000',
    '00001000',
    '10011001', // LDI R1,9  Load R1 with value 9
    '00000001',
    '00001001',
    '10101010', // MUL R0,R1 Multiply R0*R1, storing result in R0
    '00000000',
    '00000001',
    '01000011', // PRN R0    Print value in R0
    '00000000',
    '00000001', // HLT       Halt
    ];
/*

    const program = [ // print8.ls8
        "10011001", // LDI R0,8  Store 8 into R0
        "00000000",
        "00001000",
        "01000011", // PRN R0    Print the value in R0
        "00000000",
        "00000001"  // HLT       Halt and quit
    ];

    // Load the program into the CPU's memory a byte at a time
    for (let i = 0; i < program.length; i++) {
        cpu.poke(i, parseInt(program[i], 2));
    }
    */
}

/**
 * Main
 */

let ram = new RAM(256);
let cpu = new CPU(ram);

// TODO: get name of ls8 file to load from command line

loadMemory(cpu);

cpu.startClock();
