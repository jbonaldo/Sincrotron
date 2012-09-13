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
    
    GpioG1CtrlRegs.GPAMUX2.bit.GPIO20   = GPIO;
    GpioG1CtrlRegs.GPADIR.bit.GPIO20    = OUTPUT; 
    
    GpioG1CtrlRegs.GPAMUX2.bit.GPIO22 = GPIO;
    GpioG1CtrlRegs.GPADIR.bit.GPIO22    = OUTPUT;
    
    GpioG1CtrlRegs.GPAMUX2.bit.GPIO28 = GPIO;
    GpioG1CtrlRegs.GPADIR.bit.GPIO28    = OUTPUT;
    
    GpioG1CtrlRegs.GPAMUX1.bit.GPIO8 = 3;   //Toggla a cada SOCA (mostra o instante do inicio da Conv ADC)
    GpioG1CtrlRegs.GPADIR.bit.GPIO8    = OUTPUT;
    
    
    GpioG1CtrlRegs.GPCDIR.bit.GPIO70 = 1; 
    
    EDIS;
    
    GpioG1DataRegs.GPASET.bit.GPIO18 = 1;
    GPIO_ACTION(GPIO20, SET);
    
    
    
}

  