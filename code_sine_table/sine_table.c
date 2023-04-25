/*
    Generate a sine wave using a 12-bit DAC as fast as possible.
    The difficulty is on optimizing the code to generate the sine wave with
    the highest possible frequency.
    The code is optimized for the PIC12F1822 microcontroller and the DAC MCP4921.

    Function to generate:
        y = 1.5 + |3.5 * sin(w*t)|
    where the resolution of sine must be 1 degree.

    Authors: Sebastian Garcia Angarita
             Sergio Sebastian Oliveros Sepulveda
*/

const float SIN_TABLE[] = {
    0.0000,
    0.0175,
    0.0349,
    0.0523,
    0.0698,
    0.0872,
    0.1045,
    0.1219,
    0.1392,
    0.1564,
    0.1736,
    0.1908,
    0.2079,
    0.2250,
    0.2419,
    0.2588,
    0.2756,
    0.2924,
    0.3090,
    0.3256,
    0.3420,
    0.3584,
    0.3746,
    0.3907,
    0.4067,
    0.4226,
    0.4384,
    0.4540,
    0.4695,
    0.4848,
    0.5000,
    0.5150,
    0.5299,
    0.5446,
    0.5592,
    0.5736,
    0.5878,
    0.6018,
    0.6157,
    0.6293,
    0.6428,
    0.6561,
    0.6691,
    0.6820,
    0.6947,
    0.7071,
    0.7193,
    0.7314,
    0.7431,
    0.7547,
    0.7660,
    0.7771,
    0.7880,
    0.7986,
    0.8090,
    0.8192,
    0.8290,
    0.8387,
    0.8480,
    0.8572,
    0.8660,
    0.8746,
    0.8829,
    0.8910,
    0.8988,
    0.9063,
    0.9135,
    0.9205,
    0.9272,
    0.9336,
    0.9397,
    0.9455,
    0.9511,
    0.9563,
    0.9613,
    0.9659,
    0.9703,
    0.9744,
    0.9781,
    0.9816,
    0.9848,
    0.9877,
    0.9903,
    0.9925,
    0.9945,
    0.9962,
    0.9976,
    0.9986,
    0.9994,
    0.9998,
    1.0000,
    0.9998,
    0.9994,
    0.9986,
    0.9976,
    0.9962,
    0.9945,
    0.9925,
    0.9903,
    0.9877,
    0.9848,
    0.9816,
    0.9781,
    0.9744,
    0.9703,
    0.9659,
    0.9613,
    0.9563,
    0.9511,
    0.9455,
    0.9397,
    0.9336,
    0.9272,
    0.9205,
    0.9135,
    0.9063,
    0.8988,
    0.8910,
    0.8829,
    0.8746,
    0.8660,
    0.8572,
    0.8480,
    0.8387,
    0.8290,
    0.8192,
    0.8090,
    0.7986,
    0.7880,
    0.7771,
    0.7660,
    0.7547,
    0.7431,
    0.7314,
    0.7193,
    0.7071,
    0.6947,
    0.6820,
    0.6691,
    0.6561,
    0.6428,
    0.6293,
    0.6157,
    0.6018,
    0.5878,
    0.5736,
    0.5592,
    0.5446,
    0.5299,
    0.5150,
    0.5000,
    0.4848,
    0.4695,
    0.4540,
    0.4384,
    0.4226,
    0.4067,
    0.3907,
    0.3746,
    0.3584,
    0.3420,
    0.3256,
    0.3090,
    0.2924,
    0.2756,
    0.2588,
    0.2419,
    0.2250,
    0.2079,
    0.1908,
    0.1736,
    0.1564,
    0.1392,
    0.1219,
    0.1045,
    0.0872,
    0.0698,
    0.0523,
    0.0349,
    0.0175,
};

void main()
{
    unsigned int deg = 0;
    unsigned int valueDAC = 0;
    unsigned short temp1;
    unsigned short temp2;

    // Setup
    OSCCON = 0xF0;
    TRISA = 0x00;  // Set PORTA as output
    ANSELA = 0x00; // Set PORTA as digital

    PORTA.F4 = 1; // Deselect DAC chip
    SPI1_Init();

    while (1)
    {
        valueDAC = 2866.5 * SIN_TABLE[deg]; // The function is split in 2 for faster execution.
        valueDAC = 819 + valueDAC;          // 1.0 + abs(3.5 * sin(deg))

        // High Byte
        temp1 = (valueDAC >> 8) & 0x0F; // Store valueDAC[11..8] to temp[3..0]
        temp1 |= 0x30;                  // Define DAC setting, see MCP4921 datasheet
        // Low Byte
        temp2 = valueDAC; // Store valueDAC[7..0] to temp[7..0]

        PORTA.F4 = 0;      // DAC Selected, communication activated
        SPI1_Write(temp1); // Send high byte via SPI
        SPI1_Write(temp2); // Send low byte via SPI
        PORTA.F4 = 1;      // DAC deselected, communication disabled

        deg++;
        if (deg >= 180)
            deg = 0;
    }
}
