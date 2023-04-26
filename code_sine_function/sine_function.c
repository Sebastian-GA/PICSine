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

void SPI_Init()
{
    TRISA.F1 = 0; // SCK pin as output (master mode)
    TRISA.F0 = 0; // SDO pin as output
    // TRISA.F2 = 1; // SDI pin as input
    // Don't care for SDI pin because it is not used, this enables pin as General purpose Output
    TRISA.F4 = 0; // CS pin as output

    SSP1CON1 = 0b00100000; // SPI Master mode, clock = FOSC/4
    SSP1CON3 = 0b00000000; // SPI mode 0, SS pin control by software
    SSP1STAT = 0b01000000; // SMP = 0, CKE = 1
}

void main()
{
    unsigned int deg = 0;
    unsigned int valueDAC = 0; // 16-bit variable for DAC value

    // Setup
    OSCCON = 0xF0;
    TRISA = 0x00;  // Set PORTA as output
    ANSELA = 0x00; // Set PORTA as digital

    PORTA.F4 = 1; // CS=1, communication disabled
    SPI_Init();

    while (1)
    {
        valueDAC = (2866.5 * sin(0.0174533 * deg)); // The function is split in 2 for faster execution.
        valueDAC = 819 + valueDAC;                  // 1.0 + abs(3.5 * sin((PI/180)*deg))

        valueDAC |= 0x3000; // Define DAC setting, see MCP4921 datasheet

        PORTA.F4 = 0;            // DAC Selected, communication activated
        SSP1BUF = valueDAC >> 8; // Send high byte
        asm NOP;                 // Wait for transmission to complete
        asm NOP;
        asm NOP;
        asm NOP;
        asm NOP;
        SSP1BUF = valueDAC; // Send low byte
        asm NOP;            // Wait for transmission to complete
        asm NOP;
        asm NOP;
        asm NOP;
        asm NOP;
        asm NOP;
        PORTA.F4 = 1; // DAC deselected, communication disabled

        deg++;
        if (deg >= 180)
        {
            deg = 0;
            Delay_us(930); // To ensure same time per degree
                            // sin(0)=0.000 and multiplication by 0.000 is done faster
        }
    }
}