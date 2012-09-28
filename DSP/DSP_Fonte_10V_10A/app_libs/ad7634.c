 #include "DSP28x_Project.h"
 #include "F28M35x_EPwm_defines.h"  
 #include "../config.h"

void ConfigMcbspaSPIMaster(void);

void ConfigMcbspaSPIMaster(void)
{
    McbspaRegs.SPCR2.all = 0x0000;
    McbspaRegs.SPCR1.all = 0x0000;

    // Reset the McBSP
    // Disable all interrupts
    // Frame sync generator reset
    // Sample rate generator reset
    // Transmitter reset
    // Receiver reset
    McbspaRegs.SPCR2.bit.FRST = 0;
    McbspaRegs.SPCR2.bit.GRST = 0;
    McbspaRegs.SPCR2.bit.XRST = 0;
    McbspaRegs.SPCR1.bit.RRST = 0;



////////  Configura McBsp como SPI Master //////////////
    //CLKSTP configured as 1 clock delay
    //McbspaRegs.SPCR1.bit.CLKSTP = 0x03;
    McbspaRegs.SPCR1.bit.CLKSTP = 0x02;
    
    //CLKXP polarity (0: positive;  1:negative)
    McbspaRegs.PCR.bit.CLKXP = 0;
    
    //CLKRP polarity (0: positive;  1:negative)
    //McbspaRegs.PCR.bit.CLKRP = 1;
    McbspaRegs.PCR.bit.CLKRP = 0;
    
    //Transmit frame synchronization driven according FSGM bit
    McbspaRegs.PCR.bit.CLKXM = 1;
    
    //Sample rate generator input clock is LSPCLK
    McbspaRegs.SRGR2.bit.CLKSM = 1;
    McbspaRegs.PCR.bit.SCLKME = 0;
    
    //CLKGDV divider for the generated clock (CLKG)
    McbspaRegs.SRGR1.bit.CLKGDV = CLKGDV_VAL;
  
    //CLKX is driven by the sample rate generator
    McbspaRegs.PCR.bit.FSXM = 1;
    
    //The transmitter drives a frame-synchronization pulse on the FSX pin every time data is transferred from DXR1 to XSR1.
    McbspaRegs.SRGR2.bit.FSGM = 0;
    
    //FSX pin is active low
    McbspaRegs.PCR.bit.FSXP = 1;
    
    //RX data delay is 1 bit
    //TX data delay is 1 bit
    McbspaRegs.RCR2.bit.RDATDLY = 1;
    McbspaRegs.XCR2.bit.XDATDLY = 1;
//////////////////// Fim ///////////////////////////////


    // Enable Sample rate generator and
    // wait at least 2 CLKG clock cycles
    McbspaRegs.SPCR2.bit.GRST = 1;
    clkg_delay_loop();
    
    //Enable Trasmissor e receptor
    McbspaRegs.SPCR2.bit.XRST = 1;
    McbspaRegs.SPCR1.bit.RRST = 1;
    clkg_delay_loop();
    
    //Enable Frame-Sincronization Logic
    McbspaRegs.SPCR2.bit.FRST = 1;
    

    
    // McBSP uses an 20-bit word for both TX and RX
    McbspaRegs.RCR1.bit.RWDLEN1 = 3;
    McbspaRegs.XCR1.bit.XWDLEN1 = 3;
    
    
    // Reset TX and RX
    // Enable interrupts for TX and RX
    // Release TX and RX
    McbspaRegs.SPCR2.bit.XRST = 0;
    McbspaRegs.SPCR1.bit.RRST = 0;
    //McbspaRegs.MFFINT.bit.XINT = 1;
    McbspaRegs.MFFINT.bit.RINT = 1;
    
    
    //McbspaRegs.RCR1.bit.RWDLEN1 = 3;
    //McbspaRegs.XCR1.bit.XWDLEN1 = 3;
    
    
    McbspaRegs.SPCR2.bit.XRST = 1;
    McbspaRegs.SPCR1.bit.RRST = 1;
    
    
   
    /////////// MCBSP IO CONFIGURATION //////////////
    EALLOW;
    // Select one of the following for MDXA
    // GPIO20
    //GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 2;

    // MDRA
    // Qualification is asynchronous
    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 2;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 3;

    // MCLKXA
    // Qualification is asynchronous
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 2;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO22 = 3;

    // MCLKRA
    // Select one of the following
    // GPIO7 with asynchronous qualification
    // GPIO58 with asynchronous qualification
    //GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 2;
    //GpioCtrlRegs.GPAQSEL1.bit.GPIO7 = 3;

    //GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 1;
    //GpioCtrlRegs.GPBQSEL2.bit.GPIO58 = 3;
    
    
    // MFSXA
    // Qualification is asynchronous
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 2;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;
    EDIS;

}


void ad7634_config( void )
{
    ConfigMcbspaSPIMaster();
}
