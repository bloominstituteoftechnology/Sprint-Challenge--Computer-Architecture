import { parse } from "querystring";


const num1 = '11001111'

const hex1 = parseInt(num1, 2).toString(16);

console.log(hex1);

const dec1 = parseInt(num1, 2)

console.log(dec1);

const num2 = '4C'

const bin1 = parseInt(num2, 16).toString(2);

console.log(bin1);

const dec2 = parseInt(num2, 16).toString(10);

console.log(dec2);


const num3 = '68'

const bin2 = parseInt(num3, 10).toString(2);

console.log(bin2)


const hex2 = parseInt(num3, 10).toString(16);

console.log(hex2);




