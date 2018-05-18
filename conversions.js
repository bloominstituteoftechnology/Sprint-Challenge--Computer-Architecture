// Binary to Decimal

let binary = 0b11001111;
console.log(binary); // 207

let value1 = Number('0b11001111');
console.log(value1); // 207

let value2 = parseInt('11001111', 2);
console.log(value2); // 207

// Binary to Hex

let hex = parseInt('11001111', 2).toString(16);
console.log(hex); // cf

// Hex to Binary

let hex1 = 0x4c;
let binary1 = parseInt('4c', 16).toString(2);
console.log(binary1); // 1001100

// Hex to Decimal
let decimal = parseInt('4c', 16).toString(10);
console.log(decimal); // 76

// Decimal to Binary
let decimal2 = 68;
let binary2 = decimal2.toString(2);
console.log(binary2); // 1000100

// Decimal to Hex
let hex2 = decimal2.toString(16);
console.log(hex2); // 44
