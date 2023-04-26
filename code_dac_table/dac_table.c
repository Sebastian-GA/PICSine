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

const float DAC_TABLE[] = {
    819,
    869,
    919,
    969,
    1019,
    1069,
    1119,
    1168,
    1218,
    1267,
    1317,
    1366,
    1415,
    1464,
    1512,
    1561,
    1609,
    1657,
    1705,
    1752,
    1799,
    1846,
    1893,
    1939,
    1985,
    2030,
    2076,
    2120,
    2165,
    2209,
    2252,
    2295,
    2338,
    2380,
    2422,
    2463,
    2504,
    2544,
    2584,
    2623,
    2662,
    2700,
    2737,
    2774,
    2810,
    2846,
    2881,
    2915,
    2949,
    2982,
    3015,
    3047,
    3078,
    3108,
    3138,
    3167,
    3195,
    3223,
    3250,
    3276,
    3301,
    3326,
    3350,
    3373,
    3395,
    3417,
    3438,
    3458,
    3477,
    3495,
    3513,
    3529,
    3545,
    3560,
    3574,
    3588,
    3600,
    3612,
    3623,
    3633,
    3642,
    3650,
    3658,
    3664,
    3670,
    3675,
    3679,
    3682,
    3684,
    3685,
    3686,
    3685,
    3684,
    3682,
    3679,
    3675,
    3670,
    3664,
    3658,
    3650,
    3642,
    3633,
    3623,
    3612,
    3600,
    3588,
    3574,
    3560,
    3545,
    3529,
    3513,
    3495,
    3477,
    3458,
    3438,
    3417,
    3395,
    3373,
    3350,
    3326,
    3301,
    3276,
    3250,
    3223,
    3195,
    3167,
    3138,
    3108,
    3078,
    3047,
    3015,
    2982,
    2949,
    2915,
    2881,
    2846,
    2810,
    2774,
    2737,
    2700,
    2662,
    2623,
    2584,
    2544,
    2504,
    2463,
    2422,
    2380,
    2338,
    2295,
    2252,
    2209,
    2165,
    2120,
    2076,
    2030,
    1985,
    1939,
    1893,
    1846,
    1799,
    1752,
    1705,
    1657,
    1609,
    1561,
    1512,
    1464,
    1415,
    1366,
    1317,
    1267,
    1218,
    1168,
    1119,
    1069,
    1019,
    969,
    919,
    869,
};

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
        valueDAC = DAC_TABLE[deg]; // 1.0 + abs(3.5 * sin(deg))

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
            deg = 0;
    }
}
