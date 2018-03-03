/**
 * RAM access
 */
class RAM {
    constructor(size=256) {
        this.mem = new Array(size);
        this.mem.fill(0);
    }

    /**
     * Write (store) MDR value at address MAR
     */
    write(MAR, MDR) {
        // !!! IMPLEMENT ME
        // write the value in the MDR to the address MAR
       
        this.mem[MAR & (size-1)] = MDR;
    }

    /**
     * Read (load) MDR value from address MAR
     * 
     * @returns MDR
     */
    read(MAR) {
        // !!! IMPLEMENT ME
        // Read the value in address MAR and return it
   
        return this.mem[MAR & (size-1)];
    }
}

module.exports = RAM;