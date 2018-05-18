const RAM = require('./ram');
const CPU = require('./cpu');

const fs = require('fs');

let ram = new RAM(256);
let cpu = new CPU(ram);

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

for (let i = 0; i < lines.length; i++) {
  cpu.poke(i, parseInt(lines[i], 2));
}

cpu.startClock();
