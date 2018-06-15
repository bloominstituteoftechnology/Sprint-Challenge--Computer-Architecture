const fs = require('fs');

const RAM = require('./ram');
const CPU = require('./cpu');

let ram = new RAM(256);
let cpu = new CPU(ram);


const argv = process.argv.slice(2);

if (argv.length != 1) {
  console.error("usage: filename capacity");
  process.exit(1);
}

const filename = argv[0];
const filedata = fs.readFileSync(filename, "utf8");
const lines = filedata.trim().split(/[\r\n]+/g);
let idx = 0;

lines.forEach((line, i) => {
  const comment = line.split(' ')[0] === '#';
  if (!comment) {
    cpu.poke(idx++, parseInt(line.split(' ')[0], 2));
  }
});

cpu.startClock();