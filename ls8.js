const RAM = require("./ram");
const CPU = require("./cpu");

function loadMemory() {
  const data = process.argv[2];
  const fs = require("fs");
  const program = fs
    .readFileSync(data, "utf8")
    .split("\n")
    .reduce((mem, binary) => {
      if (binary.trim()[0] !== "#" && binary.trim() !== "") {
        return mem.concat([binary.slice(0, 8)]);
      } else {
        return mem;
      }
    }, []);
  for (let i = 0; i < program.length; i++) {
    cpu.poke(i, parseInt(program[i], 2));
  }
}

let ram = new RAM(256);
let cpu = new CPU(ram);

if (process.argv.length !== 3) {
  cpu.error();
} else {
  loadMemory(cpu);
  cpu.startClock();
}
