const RAM = require('./ram');
const CPU = require('./cpu');
const fileName = process.argv[2];
const fs = require('fs');
/**
 * Load an LS8 program into memory
 *
 * TODO: load this from a file on disk instead of having it hardcoded
 */
function loadMemory() {
  // Hardcoded program to print the number 8 on the console
  const program = fs
    .readFileSync(fileName)
    .toString()
    .split('\n')
    .reduce((array, line) => {
      if (line[0] !== '#' && line !== '') {
        return array.concat(line.slice(0, 8));
      }
      return array;
    }, []);

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
