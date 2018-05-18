const RAM = require('./ram');
const CPU = require('./cpu');
var fs = require('fs');
const FILE = process.argv[2];
var lineReader = require('readline').createInterface({
  input: require('fs').createReadStream(`./${FILE}.ls8`)
});

/**
 * Load an LS8 program into memory
 *
 * TODO: load this from a file on disk instead of having it hardcoded
 */
function loadMemory() {

    // Hardcoded program to print the number 8 on the console
    console.log(lineReader);
    
    const program = [ // print8.ls8
      "10011001", // # LDI R0,8
      "00000000",
      "00001000",
      "10011001", // # LDI R1,9
      "00000001",
      "00001001",
      "10101010", // # MUL R0,R1 <---
      "00000000",
      "00000001",
      "01000011", // # PRN R0
      "00000000",
      "00000001" // # HLT
    ];
    let counter = 0;
    lineReader.on('line', function (line) {
      cpu.poke(counter, parseInt(line, 2));
      console.log(line);
      counter++;
    });
    lineReader.on('close', () => {
      console.log('finished reading');
      cpu.startClock();
    });
    // Load the program into the CPU's memory a byte at a time
    // for (let i = 0; i < program.length; i++) {
    //     cpu.poke(i, parseInt(program[i], 2));
    // }
}

/**
 * Main
 */

let ram = new RAM(256);
let cpu = new CPU(ram);

// TODO: get name of ls8 file to load from command line

loadMemory(cpu);
