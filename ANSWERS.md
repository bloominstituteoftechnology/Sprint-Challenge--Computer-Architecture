<!-- Answers to the Short Answer Essay Questions go here -->

### Binary, Decimal, and Hex 

---

* Convert `11001111` binary

    to hex:

    to decimal:

Before we do any conversion, let's break down `11001111`:

|-|2<sup>7</sup>|2<sup>6</sup>|2<sup>5</sup>|2<sup>4</sup>|2<sup>3</sup> |2<sup>2</sup>|2<sup>1</sup>|2<sup>0</sup>|
|:---: |:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|*Binary*|1|1|0|0|1|1|1|1|
|*Decimal Eqv*|128|64|0|0|8|4|2|1|
|*Hex Eqv*|80|40|0|0|8|4|2|1

To get the hexadecimal equivalent, we'll just add all the hex numbers in the table:

```
  80
  40
   0
   0
   8
   4
   2
+  1
____
  CF
```

This is base 16 addition, so `8 + 4 + 2 + 1` is `F`. Similarly, `8 + 4` is `C`.

**Hex**: **`CF`**

For decimal, we'll just add all the decimal numbers in the table:

```
 128
  64
   0
   0
   8
   4
   2
+  1
____
 207
```

**Decimal**: **`207`**

---

* Convert `4C` hex

    to binary:

    to decimal:

Let's do a table. I love tables!

|-|16<sup>1</sup>|16<sup>0</sup>|
|:--:|:--:|:--:|
|*Hex*|4|C|
|*Binary*|(`100 * 10000`)|(`1100 * 1`)|
|*Decimal*|(`4 * 16`)|(`12 * 1`)|

In decimal, the hex number can be parsed as

"Four `16`'s and Thirteen `1`'s".

Specifically, the first digit shows how many `16s` are there in the number, while the second digit shows how many `1`s are in the the number. This is analogous to decimal, where each digit denotes how many `10`'s there are. For example, decimal `68` can be alternatively read as "6 `10`'s and 8 `1`'s".

So, let's get it:

```
(4 * 16) + (12 * 1) = x
    64   +     12   = x
        76          = x
```

**Decimal**: **`77`**

Binary is similar in process:

"`100` (4) `10000` (`16`'s) and `1100` (12) `1`'s"

This might be a harder way to do it, but I can't think of an easier one, so let's get this out of the way:

```
(Binary 4 * Binary 16)
   10000
     100
  x_____
   00000
  00000
 10000    
 _______
 1000000

 (Binary 13 * Binary 1)
   1100
      1
 x_____
   1100

(Binary 64 + Binary 13)
 1000000
    1100
 +______
 1001100
```

**Binary**: **`1001101`**

#### Answer

```
Hex:        4C
Decimal:    76
Binary:     1001100
```

---

* Convert `68` decimal

    to binary:

    to hex:

To convert `68` to binary, we can think about what powers of two are in the number:

|-|2<sup>6</sup>|2<sup>5</sup>|2<sup>4</sup>|2<sup>3</sup> |2<sup>2</sup>|2<sup>1</sup>|2<sup>0</sup>|
|:---:|:---: |:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|*Dec. Eqv.*|*64*|*32*|*16*|*8*|*4*|*2*|*1*|
|In there?|Yes|No|No|No|Yes|No|No|
|Binary |1|0|0|0|1|0|0|

To break it down, we think of the highest power of two we can divide into the decimal and get a result that equals 1 or more. 64, or 2<sup>6</sup>, can go into `68`, but not 128, or 2<sup>7</sup>. So in our table, we'll set the highest power of two to 2<sup>6</sup>. Then we say `Yes` in our table, as in `Yes`, there's a 2<sup>6</sup> in there.

Then we subtract 64 from our `68`, leaving us with `4` left over. Then we do the process for each preceding power of 2. So is 2<sup>5</sup>/32 in `4`? Nope. Put `No` then move on. Is 2<sup>4</sup>/16 in `4`? Nope. Put `No` then move on. We do this until reach 2<sup>0</sup>.

Then look at your mighty table, pretend the `Yes`'s are `1`'s and `No`'s are `0`'s, and *voila*, you got your binary number.

We'll do something similar for hex conversion. This time, we think of what powers of *16* are in the decimal number. However, instead of asking if it's there, we'll ask how many are there. This is because, unlike binary, there could be multiples of a given power of 16 in the number.

|-|16<sup>1</sup>|16<sup>0</sup>|
|:---:|:---:|:---:|
|*Dec Eqv*|16|1|
|How many?|4|4|

Each digit in `How Many?` happens to be our hex number: `44`. If `How Many?` is greater than 9, convert the number into its hex equivalent, such as decimal `77` in an earlier example converts to `4C`. If your `How Many?` is greater than 15, you didn't list enough powers of 16 in the table.

**Hex**: **`44`**

#### Answer

```
Decimal:    68
Binary:     1000100
Hex:        44
```

### Architecture

1. Explain how the CPU provides concurrency:

Concurrency is established through *multi-threading*. In multi-threading, threads share the resources of a core(s) in a CPU. A thread is "the smallest thread of execution that can be managed by a scheduler [in the OS]". 

From the little that I read, and even less of what I understand, multi-threading seems to be try to maximise the utilisation of the CPU by performing other instructions while another instruction is pending. A possible example might be if a given instruction needs to access something in RAM, which takes a relatively long time. Perhaps during that time, the CPU can do another instruction or two while that is resolving. (*I might be totally wrong though.*)

![multithreadimg](https://upload.wikimedia.org/wikipedia/commons/a/a5/Multithreaded_process.svg)

As we discussed last week, the CPU can give a _feeling_ of processes running at the same time through scheduling, which the OS takes care of. To my understanding, the CPU isn't necessarily doing multiple things at the same time, but is rather rapidly switching the one thing it is working on, doing a bit of work on each many many times per second.

*Resource used*: [Multithreading (computer architecture)](https://en.wikipedia.org/wiki/Multithreading_(computer_architecture))

2. Describe assembly language and machine language:

Machine language is the pure binary code that the CPU can understand and process. The `0`'s and `1`s can be converted to equivalent electrical signals that can be received by the CPU and can be sent to various components of the computer.

However, we don't process data that way, so reading `0`'s and `1`s is a little crazy. This is where assembly comes in. The assembly language is one abstraction level up from machine language.

What makes assembly language different from other languages like C or JS is that it's a direct and faithful translation of machine code. Basically, each machine code instruction is directly translated into simple commands, values, and register addresses that we can better understand. But that's it: it's only human-readable machine code.

In contrast, most other programming languages further abstract and simplify common patterns of operation. For example:

```js
function mult2print(x) {
    console.log(x + x);
}

mult2print(10);

mult2print(15);

mult2print(18);

mult2print(30);
```

This is (*my best guess of*) the JS approximation of `call.ls8`. Compare this to assembly language:

```
LDI     R1,MULT2PRINT
LD1     R0,10
CALL    R1
LDI     R0,15
CALL    R1
LDI     R0,18
CALL    R1
LDI     R0,30
CALL    R1
HLT
#MULT2PRINT
ADD     R0,R0
PRN     R0
RET
```

In the JS language, the language is structured to allow programmers to better read, write, and reason about the program. However, in the assembly version, you'd have to pretend you're the CPU for a bit to get a handle of what this thing is doing. But the C program would need more (perhaps many more) steps for this human-readable program to be converted to machine code, whereas the assembly language is much more straightfoward in translating this to machine code.