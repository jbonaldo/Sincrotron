 #include "DSP28x_Project.h"
 #include "F28M35x_EPwm_defines.h"  
 #include "../config.h"




void InitMcbspa(void)
{

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

    McbspaRegs.SPCR2.all = 0x0000;
    McbspaRegs.SPCR1.all = 0x0000;
    
    McbspaRegs.SPCR1.bit.CLKSTP = 11b;
    McbspaRegs.SPCR1.bit.
    
    
    

    // RX data delay is 1 bit
    // TX data delay is 1 bit
    McbspaRegs.RCR2.bit.RDATDLY = 1;
    McbspaRegs.XCR2.bit.XDATDLY = 1;

    // No clock sync for CLKG
    // Frame-synchronization period
    McbspaRegs.SRGR2.bit.GSYNC = 0;
    McbspaRegs.SRGR2.bit.FPER = 320;

    // Frame-synchronization pulses from
    // the sample rate generator
    McbspaRegs.SRGR2.bit.FSGM = 1;

    // Sample rate generator input clock is LSPCLK
    McbspaRegs.SRGR2.bit.CLKSM = 1;
    McbspaRegs.PCR.bit.SCLKME = 0;

    // Divide-down value for CLKG
    // Frame-synchronization pulse width
    McbspaRegs.SRGR1.bit.CLKGDV = CLKGDV_VAL;
    clkg_delay_loop();
    McbspaRegs.SRGR1.bit.FWID = 1;

    // CLKX is driven by the sample rate generator
    // Transmit frame synchronization generated by internal
    // sample rate generator
    McbspaRegs.PCR.bit.CLKXM = 1;
    McbspaRegs.PCR.bit.FSXM = 1;

    // Enable Sample rate generator and
    // wait at least 2 CLKG clock cycles
    McbspaRegs.SPCR2.bit.GRST = 1;
    clkg_delay_loop();

    // Release from reset
    // RX, TX and frame sync generator
    McbspaRegs.SPCR2.bit.XRST = 1;
    McbspaRegs.SPCR1.bit.RRST = 1;
    McbspaRegs.SPCR2.bit.FRST = 1;
    
    
    // McBSP uses an 20-bit word for both TX and RX
    McbspaRegs.RCR1.bit.RWDLEN1 = 3;
    McbspaRegs.XCR1.bit.XWDLEN1 = 3;
    
    
    
    // Reset TX and RX
    // Enable interrupts for TX and RX
    // Release TX and RX
    McbspaRegs.SPCR2.bit.XRST = 0;
    McbspaRegs.SPCR1.bit.RRST = 0;
    McbspaRegs.MFFINT.bit.XINT = 1;
    McbspaRegs.MFFINT.bit.RINT = 1;
    McbspaRegs.SPCR2.bit.XRST = 1;
    McbspaRegs.SPCR1.bit.RRST = 1;
    
    
    
    // Config McBsp IO
    
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
}