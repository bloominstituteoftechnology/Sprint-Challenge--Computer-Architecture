/**
 * RAM access
 */
class RAM {
    constructor(size) {
        this.mem = new Array(size);
        this.mem.fill(0);
        //inintallizes memory to all zeros
        //in cpu.js poke uses this.ram.write
        //but fill is used in ls8.js
    }

    /**
     * Write (store) MDR value at address MAR
     */
    write(MAR, MDR) {
        // !!! IMPLEMENT ME
        // write the value in the MDR to the address MAR
        //Memory address register, memory data register
        // MAR holds the address of instruction, MDR holds the the value
        this.mem[MAR] = MDR;
    }

    /**
     * Read (load) MDR value from address MAR
     * 
     * @returns MDR
     */
        read(MAR) {
        // !!! IMPLEMENT ME
        // Read the value in address MAR and return it
        return this.mem[MAR];
    }
}

module.exports = RAM;