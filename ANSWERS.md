# Conversion Questions

1. Convert `11001111` binary

    to hex: 207 / 16 = 12.9375
            16 * 12 = 192
            207 - 192 = 15
            Twelve "16's" or C
            Fifteen "1's" or F
            `0xCF`

            Or, another way to solve it is to break the binary into two hexadigits, solve each one, and combine:

            1100 = 12 (or C), 1111 = 15 (or F)
            Combine the two hexadigits and you get `0xCF`

    to decimal: 1 + 2 + 4 + 8 + 0 + 0 + 64 + 128 = `207`


2. Convert `4C` hex

    to binary: 4 * 16 = 64 = 0100. 12 * 1 = 12 = 1100.
    Combine the two hexadigits and you get `01001100`

    to decimal: 4 * 16 = 64. 12 * 1 = 1. 
    64 + 12 = `76`


3. Convert `68` decimal:

    to binary: one 64 and one 4 = `01000100`

    to hex: 0100 = 4 (in hex). Add two of those hexadigits = `0x44`

# Short Answer Essay Questions

1. Explain how the CPU provides concurrency:

   By having different points of execution for each computation, or process.

2. Describe assembly language and machine language:

   Machine language is at the lowest level in the form of bits and bytes, 1's and 0's. Eight bits i.e. 10101010 forms one byte. Either a voltage is coming through for a `1` or it isn't for a `0`.

   Assembly language is a step higher from machine language. It uses more symbols and is easier to understand and work with than machine language.

3. Why is 3D performance so much higher with a graphics card than without?

   Graphics cards have a graphics processor unit (GPU) which are specialized in handling 2D and 3D calculations. The CPU can pass on these calculations to the GPU, which is much faster at processing these calculations than the CPU.

4. Suggest the role that graphics cards play in machine learning:

   Machine learning can require vast amounts of data and processes to complete. If these processes include 2D and 3D calculations than a graphics card will make the machine learning faster, which in turn can perform more iterations on whichever algorithm is being implemented.
