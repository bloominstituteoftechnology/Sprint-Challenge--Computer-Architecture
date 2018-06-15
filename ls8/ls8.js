const RAM = require("./ram");
const CPU = require("./cpu");

// File System
const fs = require("fs");

/**
 * Load an LS8 program into memory
 *
 * TODO: load this from a file on disk instead of having it hardcoded
 */
function loadMemory() {
  // Hardcoded program to print the number 8 on the console

  const print8 = [
    // print8.ls8, should print 8
    "10011001", // LDI R0,8  Store 8 into R0
    "00000000",
    "00001000",
    "01000011", // PRN R0    Print the value in R0
    "00000000",
    "00000001" // HLT       Halt and quit
  ];

  const mult = [
    // mult.ls8, should print 72
    "10011001", // LDI R0,8  Load R0 with value 8
    "00000000",
    "00001000",
    "10011001", //LDI R1,9  Load R1 with value 9
    "00000001",
    "00001001",
    "10101010", // MUL R0,R1 Multiply R0*R1, storing result in R0
    "00000000",
    "00000001",
    "01000011", //PRN R0    Print value in R0
    "00000000",
    "00000001" // HLT       Halt
  ];

  // Load the program into the CPU's memory a byte at a time
  //   for (let i = 0; i < program.length; i++) {
  //     cpu.poke(i, parseInt(program[i], 2));
  //   }
}

/**
 * Main
 */

let ram = new RAM(256);
let cpu = new CPU(ram);

// TODO: get name of ls8 file to load from command line
// process.argv[0] === "node"
// process.argv[1] === "ls8"
// process.argv[2] === "mult.ls8"

/* 1st attempt 
 const filename = process.argv[2];
const filedata = fs.readFileSync(filename, "utf8");
const lines = filedata.trim().split(/[\r\n]+/g);

lines.forEach((value, index) => {
  cpu.poke(index, parseInt(value, 2));
});
*/

const args = process.argv;

   try {
       const regexp = /[0-9]{8}/g;
       const program = fs.readFileSync(`${args[2]}`, "utf-8").match(regexp);

       //Load the program into the CPU's memory a byte at a time
       for (let i = 0; i < program.length; i++) {
           cpu.poke(i, parseInt(program[i], 2));
       }
   }
   catch (err) {
       console.log('invalid file, try again');
       process.exit();
   }


loadMemory(cpu);

cpu.startClock();
