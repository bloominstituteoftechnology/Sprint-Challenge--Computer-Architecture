/**
 * RAM access
 */
class RAM {
    constructor(size) {
        this.mem = new Array(size);
        this.mem.fill(0);
    }

    /**
     * Write (store) MDR value at address MAR
     */
    write(MAR, MDR) {
        this.mem[MAR] = MDR;

        // let foo = MAR.concat(MDR[i];
        // !!! IMPLEMENT ME
        // write the value in the MDR to the address MAR
    }

    /**
     * Read (load) MDR value from address MAR
     * 
     * @returns MDR
     */
    read(MAR) {
        // const readMar = (MAR) => {
            // return JSON.stringify(MAR);
        // }

        return this.mem[MAR];
        // !!! IMPLEMENT ME
        // Read the value in address MAR and return it
    }
}

module.exports = RAM;