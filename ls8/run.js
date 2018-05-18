const fs = require('fs');

const argv = process.argv.slice(1);

let filename = argv[1];
if (!filename) {
  console.log('usage: [filename]');
  process.exit(1);
}
if (!filename.endsWith('.ls8')) {
  filename = filename + '.ls8';
}

const filedata = fs.readFileSync(filename, 'utf8');

const lines = filedata
  .split('\n')
  .filter(line => line.startsWith('0') || line.startsWith('1'))
  .map(line => line.split('#'))
  .map(line => line[0].trim());

const RAM = require('./ram');
const CPU = require('./cpu');
// console.log(lines);
/**
 * Load an LS8 program into memory
 *
 * TODO: load this from a file on disk instead of having it hardcoded
 */
function loadMemory() {
  // Hardcoded program to print the number 8 on the console

  const program = lines;
  // program.forEach((item, index) => {
  //   console.log(index, item);
  // });
  // [
  //   // print8.ls8
  //   '10011001', // LDI R0,8  Store 8 into R0
  //   '00000000',
  //   '00001000',
  //   '01000011', // PRN R0    Print the value in R0
  //   '00000000',
  //   '00000001', // HLT       Halt and quit
  // ];
  // console.log(program);

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
