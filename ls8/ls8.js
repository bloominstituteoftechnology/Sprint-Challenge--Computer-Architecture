const fs = require('fs');
const RAM = require('./ram');
const CPU = require('./cpu');

/**
 * Load an LS8 program into memory
 *
 * TODO: load this from a file on disk instead of having it hardcoded
 */
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

// TODO: get name of ls8 file to load from command line

loadMemory(cpu);

cpu.startClock();
