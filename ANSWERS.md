<!-- Answers to the Short Answer Essay Questions go here -->

### Binary, Decimal, and Hex 
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

* Convert `4C` hex

    to binary:

    to decimal:

Let's do a table. I love tables!

|-|16<sup>1</sup>|16<sup>0</sup>|
|:--:|:--:|:--:|
|*Hex*|4|C|
|*Binary*|(`100 * 10000`)|(`1101 * 1`)|
|*Decimal*|(`4 * 16`)|(`13 * 1`)|



* Convert `68` decimal

    to binary:

    to hex:

### Architecture

1. Explain how the CPU provides concurrency:



2. Describe assembly language and machine language:


