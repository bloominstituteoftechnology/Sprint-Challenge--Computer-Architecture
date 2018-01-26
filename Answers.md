#Complete the following problems:

* Convert `11001111` binary

- to hex: CF
 
```
const binToHex = (str) => { return parseInt(str, 2).toString(16).toUpperCase(); };
console.log(binToHex('11001111'));
```
- to decimal: 207

```
const binToDec = (str) => { return parseInt(str, 2); };
console.log(binToDec('11001111'));
```

* Convert `4C` hex

- to binary: 01001100
```
 const hexToBin = (str) => { return ('00000000' + (parseInt(str, 16)).toString(2)).substr(-8); };
 console.log(hexToBin('4C'));
```
 - to decimal: 76
    
```
const hexToDec = (str) => { return parseInt(str, 16); };
console.log(hexToDec(`4C`));
```

* Convert `68` decimal

  - to binary: 1000100
```
const decToBin = (num) => { return num.toString(2); }; 
console.log(decToBin(68)); 
```
   
  - to hex: 44 

```
const decToHex = (num) => { return num.toString(16); }; 
console.log(decToHex(68)); 
```


## Architecture

One paragraph-ish:

* Explain how the CPU provides concurrency:
  - `Single Core/non-distrubted:` threads and bus achictecture divide up the work.
    Absent such hardware, the cpu can interleave a schedule of instructions to
    share and manage resources.
  - `Multicore/distributed systems:` threads and servers divide up the work load 
    per node or core.

* Describe assembly language and machine language:
  - `Machine language (code):` a low level language (consisting of binary or hex) that 
    higher level languages compile down into. Generally this is the level just above 
    vendor based hardware instructions or microcode.
  - `Assembly language:` assembles machine code. Uses opcodes to manage and interface
    with hardware instructions. Ideal for specialized hardware tasks that removes 
    much of the cruft and safeguards. Generally the lowest level a human programs 
    outside of hacks and analog methods.
* Suggest the role that graphics cards play in machine learning:
  - Offloading intensive processing to more specialized hardware and APIs that can
    also effciently and cost-effectively link together. 
      - nVidia in particular has Tensor Cores that operate as AI hardware first and 
        foremost with the DGX brand of cards.
