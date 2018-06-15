const fs = require('fs');

const RAM = require('./ram');
const CPU = require('./cpu');

function loadMemory() {
    const program = [];
    const argv = process.argv;
    if (argv.length !== 3) {
      console.error('Usage: Node ls8 [filename]');
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

    for (let i = 0; i < program.length; i++) {
        cpu.poke(i, parseInt(program[i], 2));
    }
}

let ram = new RAM(256);
let cpu = new CPU(ram);

loadMemory(cpu);

cpu.startClock();