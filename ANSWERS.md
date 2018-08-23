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
|*Binary*|(`100 * 10000`)|(`1101 * 1`)|
|*Decimal*|(`4 * 16`)|(`13 * 1`)|

In decimal, the hex number can be parsed as

"Four `16`'s and Thirteen `1`'s".

Specifically, the first digit shows how many `16s` are there in the number, while the second digit shows how many `1`s are in the the number. This is analogous to decimal, where each digit denotes how many `10`'s there are. For example, decimal `68` can be alternatively read as "6 `10`'s and 8 `1`'s".

So, let's get it:

```
(4 * 16) + (13 * 1) = x
    64   +     13   = x
        77          = x
```

**Decimal**: **`77`**

Binary is similar in process:

"`100` (4) `10000` (`16`'s) and `1101` (13) `1`'s"

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
   1101
      1
 x_____
   1101

(Binary 64 + Binary 13)
 1000000
    1101
 +______
 1001101
```

**Binary**: **`1001101`**

#### Answer

```
Hex:        4C
Decimal:    77
Binary:     1001101
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

To break it down, we think of the highest power of two we can divide into the decimal number that equals 1 or more. 64, or 2<sup>6</sup>, can go into `68`, but not 128, or 2<sup>7</sup>. So in our table, we'll set the highest power of two to 2<sup>6</sup>. Then we say `Yes` in our table, as in `Yes`, there's a 2<sup>6</sup> in there.

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



2. Describe assembly language and machine language:


