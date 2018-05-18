const RAM = require('./ram');
const CPU = require('./cpu');

const fs = require('fs');
const node = process.argv[0];
const ls8 = process.argv[1];
const filename = process.argv[2];

function loadMemory() {
  const program = fs
    .readFileSync(filename)
    .toString()
    .split('\n')
    .reduce((array, line) => {
      if (line[0] !== '#' && line[0] !== '\r' && line !== '') {
        return array.concat(line.slice(0, 8));
      }
      return array;
    }, []);

  for (let i = 0; i < program.length; i++) {
    cpu.poke(i, parseInt(program[i], 2));
  }
}

let ram = new RAM(256);
let cpu = new CPU(ram);

// TODO: get name of ls8 file to load from command line

loadMemory(cpu);

cpu.startClock();
