const RAM = require("./ram");
const CPU = require("./cpu");

/**
 * Load an LS8 program into memory
 *
 * TODO: load this from a file on disk instead of having it hardcoded
 */
const fs = require("fs");

const argv = process.argv.slice(2);

if (argv.length !== 1) {
  console.error("Add a valid file to read instructions from");
  process.exit(1);
}

let filename = argv[0];
if (!filename.includes(".ls8")) {
  filename += ".ls8";
}

const filedata = fs.readFileSync(filename, "utf8");
const lines = filedata.trim().split(/[\r\n]+/g);

const items = [];

for (let l of lines) {
  if (l[0] !== "#") {
    items.push(l.substring(0, 8));
  }
}

function loadMemory() {
  // Hardcoded program to print the number 8 on the console

  //   const program = [
  //     // print8.ls8
  //     "10011001", // LDI R0,8  Store 8 into R0
  //     "00000000",
  //     "00001000",
  //     "01000011", // PRN R0    Print the value in R0
  //     "00000000",
  //     "00000001" // HLT       Halt and quit
  //   ];

  const program = items;
  console.log("These are the instructions given:\n", program);

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
