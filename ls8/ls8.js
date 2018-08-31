const RAM = require('./ram');
const CPU = require('./cpu');

const fs = require('fs');

function loadMemory() {
  const filename = process.argv[2];

  const program = fs.readFileSync(filename)
    .toString()
    .split('\n')
    .reduce((memo, line) => {
      if (line[0] !== '#' && line[0] !== '') {
        return memo.concat(line.slice(0, 8));
      }
      return memo;
    }, [])
    .filter(line => line !== '');

    for (let i = 0; i < program.length; i++) {
      cpu.poke(i, parseInt(program[i], 2));
    }
}

let ram = new RAM(256);
let cpu = new CPU(ram);

loadMemory(cpu);

cpu.startClock();
