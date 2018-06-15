const RAM = require("./ram");
const CPU = require("./cpu");

const ram = new RAM(256);
const cpu = new CPU(ram);

/* PROCESS THE DATA FILE AND FILTER OUT THE NOISE */
const fs = require("fs");

const filename = process.argv[2];

const filedata = fs.readFileSync(filename, "utf8").split(/[\r\n]+/g);

const program = filedata.reduce((acc, line) => {
  return line[0] === "0" || line[0] === "1" ? [...acc, line.substr(0, 8)] : acc;
}, []);

/* STORE EACH BINARY INSTRUCTION IN RAM VIA CPU */
program.forEach((value, index) => cpu.writeToRAM(index, parseInt(value, 2)));

/* FIRE UP THE CPU */
cpu.startClock();