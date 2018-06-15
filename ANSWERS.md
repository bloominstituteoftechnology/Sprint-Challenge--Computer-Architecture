**1a.** Convert `11001111` binary
  ```
  128 + 64 + 0 + 0 + 8 + 4 + 2 + 1 = 207

  HEX: CF

  DECIMAL: 207
  ```

**1b.** Convert `4C` hex
  ```
  4 = 4, C = 12

  4 + 12 = 16

  4 * 16^1 = 64
  C * 16^0 = 12

  64 + 12 = 76

  BINARY: 1001100

  DECIMAL: 76
  ```

**1c.** Convert `68` decimal
  ```
  BINARY: 1000100

  HEX: 44
    1. divide 68 by 16 (base16) until the quotient is 0
        68 / 16 = 4 (remainer 4)
        4 / 16 = 0  (remainer 4)

    2. get the hex values for each remainder 
        decimal 4 = hex 4
        decimal 4 = hex 4

    3. combine the hex values in reverse order (last to first)
        hex 4 combined with hex 4 = hex 44
  ```

**2.** Explain how the CPU provides concurrency:
  * concurrency occurs when two tasks can execute, run, and complete in overlapping time periods. for example, loading multiple browser tabs at the same time. the cpu provides this added benefit by working independently of other cpu's within the same machine. alternatively, interrupts seem to provide this advantage to cpu's as well.


**3.** Describe assembly language and machine language:
  * machine language is essentially just a bunch of binary numbers that a computer can read then execute the corresponding action.
  * assembly language provides a more intuitive way to read and write machine language. the instructions and registers are given labels for simplified future reference. assembly language is fairly comparable to machine language.


**4.** Why is 3D performance so much higher with a graphics card than without?


**5.** Suggest the role that graphics cards play in machine learning:
