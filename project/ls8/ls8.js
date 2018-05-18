const RAM = require("./ram");
const CPU = require("./cpu");

function loadMemory() {
  const program = [
    10011001, //# LDI R0,10
    00000000,
    00001010,
    10011001, //# LDI R1,20
    00000001,
    00010100,
    10011001, //# LDI R2,TEST1
    00000010,
    00010011,
    10100000, //# CMP R0,R1
    00000000,
    00000001,
    01010001, //# JEQ R2        Does not jump because R0 != R1
    00000010,
    10011001, //# LDI R3,1
    00000011,
    00000001,
    01000011, //# PRN R3        Prints 1
    00000011,

    //# TEST1 (19):
    10011001, //# LDI R2,TEST2
    00000010,
    00100000,
    10100000, //# CMP R0,R1
    00000000,
    00000001,
    01010010, //# JNE R2        Jumps because R0 != R1
    00000010, //
    10011001, // # LDI R3,2
    00000011, //
    00000010, //
    01000011, // # PRN R3        Skipped--does not print
    00000011, //

    //# TEST2 (32):
    10011001, // # LDI R1,10
    00000001, //
    00001010, //
    10011001, // # LDI R2,TEST3
    00000010, //
    00110000, //
    10100000, // # CMP R0,R1
    00000000, //
    00000001, //
    01010001, // # JEQ R2        Jumps becuase R0 == R1
    00000010, //
    10011001, // # LDI R3,3
    00000011, //
    00000011, //
    01000011, // # PRN R3        Skipped--does not print
    00000011, //

    //# TEST3 (48):
    10011001, // # LDI R2,TEST4
    00000010, //
    00111101, //
    10100000, // # CMP R0,R1
    00000000, //
    00000001, //
    01010010, // # JNE R2        Does not jump because R0 == R1
    00000010, //
    10011001, // # LDI R3,4
    00000011, //
    00000100, //
    01000011, // # PRN R3        Prints 4
    00000011, //

    //# TEST4 (61):
    10011001, // # LDI R3,5
    00000011, //
    00000101, //
    01000011, // # PRN R3        Prints 5
    00000011, //
    10011001, // # LDI R2,TEST5
    00000010, //
    01001001, //
    01010000, // # JMP R2        Jumps unconditionally
    00000010, //
    01000011, // # PRN R3        Skipped-does not print
    00000011, //

    //# TEST5 (73):
    00000001 //# HLT
  ];
  for (let i = 0; i < program.length; i++) {
    cpu.poke(i, parseInt(program[i], 2));
  }
}

let ram = new RAM(256);
let cpu = new CPU(ram);

loadMemory(cpu);
cpu.startClock();

/**
 * Load an LS8 program into memory
 *
 * TODO: load this from a file on disk instead of having it hardcoded
 */

// Hardcoded program to print the number 8 on the console

// print8.ls8

// LDI R0,8  Store 8 into R0

// PRN R0    Print the value in R0

// HLT       Halt and quit

// Load the program into the CPU's memory a byte at a time

/**
 * Main
 */

// TODO: get name of ls8 file to load from command line
