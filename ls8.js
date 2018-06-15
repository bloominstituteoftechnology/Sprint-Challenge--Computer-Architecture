const fs = require("fs");
const RAM = require("./ram");
const CPU = require("./cpu");

/**
 * Load an LS8 program into memory
 *
 * TODO: load this from a file on disk instead of having it hardcoded
 */
function readFile() {
  // Where to get the filename from
  const filename = process.argv[2];

  // Read the file
  const filedata = fs.readFileSync(filename, "utf8");

  // Split the filedata on each new line
  const lines = filedata.trim().split(/[\r\n]+/g);

//   for (let eachLine of lines) {
//     console.log("pre:", eachLine);
//     if (eachLine[0] === "0" || eachLine[0] === "1") {
//       eachLine.slice(0, 7);
//       //   eachLine.substr(0, 8);
//     } else {
//     }
//     console.log("post:", eachLine);
//   }

    const program = lines.reduce((acc, line) => {
      return line[0] === "0" || line[0] === "1"
        ? [...acc, line.substr(0, 8)]
        : acc;
    }, []);

  program.forEach((value, index) => cpu.poke(index, parseInt(value, 2)));
}

function loadMemory() {
    const program = [
  // Hardcoded program to print the number 8 on the console
  // print8.ls8
  // "10011001", // LDI R0,8  Store 8 into R0
  // "00000000",
  // "00001000",
  // "01000011", // PRN R0    Print the value in R0
  // "00000000",
  // "00000001"  // HLT       Halt and quit

  // mult.ls8
  "10011001", // LDI R0,8
  "00000000",
  "00001000",
  "10011001", // LDI R1,9
  "00000001",
  "00001001",
  "10101010", // MUL R0,R1 <---
  "00000000",
  "00000001",
  "01000011", // PRN R0
  "00000000",
  "00000001" // HLT
    ];

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

// loadMemory(cpu); // hardcode
readFile(cpu); // read from the file itself

cpu.startClock();
