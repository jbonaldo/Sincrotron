/**
 * gpio_config.c
 * 
 * Rotinas para automatizar a configuração de pinos do DSP como GPIOs
 * 
 * Desenvolvidor por:   Jakson Bonaldo
 * Data:                23/05/2012
 */
 
 
 #include "DSP28x_Project.h"
 #include "includes/gpio_config.h"
 
 void xInt_Config();
 
// #define GPIO_CONFIG(gpio, funcao)              \
//            if(gpio > 15)
//                GpioG1CtrlRegs.GPAMUX2.bit.#gpio = funcao
//            else
//                GpioG1CtrlRegs.GPAMUX1.bit.#gpio = funcao
//        
// 
// 
// void gpio_config(Uint16 gpio, Uint16 funcao )
// {
//    Uint16 mascara;
//    
//    mascara = 0xFFFF;
//
//        
//        
//        
// }




void gpio_config(void)
{
    EALLOW;
   
    GpioG1CtrlRegs.GPAMUX1.bit.GPIO10   = GPIO;
    GpioG1CtrlRegs.GPADIR.bit.GPIO10    = OUTPUT; 
     
    //Interrupcao indicando fim de conversao do AD7634 
    GpioG1CtrlRegs.GPAMUX2.bit.GPIO20   = GPIO;
    GpioG1CtrlRegs.GPADIR.bit.GPIO20    = INPUT; 
    
    GpioG1CtrlRegs.GPAMUX1.bit.GPIO15   = GPIO;
    GpioG1CtrlRegs.GPADIR.bit.GPIO15    = OUTPUT;
    
    GpioG1CtrlRegs.GPAMUX2.bit.GPIO28 = GPIO;
    GpioG1CtrlRegs.GPADIR.bit.GPIO28    = OUTPUT;
    
    GpioG1CtrlRegs.GPAMUX1.bit.GPIO8 = 3;   //Toggla a cada SOCA (mostra o instante do inicio da Conv ADC)
    GpioG1CtrlRegs.GPADIR.bit.GPIO8    = OUTPUT;
    
    
    GpioG1CtrlRegs.GPCDIR.bit.GPIO70 = 1; 
    
    EDIS;
    
    GpioG1DataRegs.GPASET.bit.GPIO18 = 1;
    
    
    xInt_Config();
    
    
    
}

void xInt_Config()
{
    EALLOW; 
    GpioG1CtrlRegs.GPAMUX2.bit.GPIO20   = GPIO;
    GpioCtrlRegs.GPADIR.bit.GPIO20 = 0;   // GPIO20 = input
    GpioCtrlRegs.GPAQSEL2.bit.GPIO20 = 0; // sincronizado com SYSCLKOUT
    GpioCtrlRegs.GPACTRL.bit.QUALPRD3 = 0;    //Periodo de amostragem igual ao de SYSCLKOUT

                                                                                                                                                                                                              
    GpioG1TripRegs.GPTRIP5SEL.bit.GPTRIP5SEL = 20; //Map Trip Input 5(XINT2) to GPIO20
    EDIS;

    XIntruptRegs.XINT2CR.bit.POLARITY = 0 ;      // Gera interrup na borda de descida
    
    XIntruptRegs.XINT2CR.bit.ENABLE = 1;        // Enable Xint1
} 



  