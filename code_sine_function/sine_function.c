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

void main()
{
    unsigned int deg = 0;
    int valueDAC = 0;
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
        valueDAC = 819 + (2866.5 * sin(0.0174533*deg)); // 1.0 + abs(3.5 * sin((PI/180)*deg))

        // High Byte
        temp1 = (valueDAC >> 8) & 0x0F; // Store valueDAC[11..8] to temp[3..0]
        temp1 |= 0x30;                  // Define DAC setting, see MCP4921 datasheet
        // Low Byte
        temp2 = valueDAC; // Store valueDAC[7..0] to temp[7..0]

        PORTA.F4 = 0;      // Select DAC chip
        SPI1_Write(temp1); // Send high byte via SPI
        SPI1_Write(temp2); // Send low byte via SPI
        PORTA.F4 = 1;      // Deselect DAC chip

        deg++;
        if (deg >= 180)
            deg = 0;
    }
}