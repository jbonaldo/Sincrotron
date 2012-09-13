/**
 *  Código para DSP F28M35 para controle da fonte 10V-10A
 * 
 *  Criado por: Jakson Bonaldo - jbonaldo@gmail.com
 *  Data:       22/05/2012
 */

#include "DSP28x_Project.h"     // F28M35x Headerfile Include File
#include "app_libs/includes/gpio_config.h"
#include "app_libs/includes/pwm_config.h"
#include "app_libs/includes/adc_config.h"
#include "app_libs/includes/controller_pi.h"
#include "app_libs/includes/shared_mem.h"
#include "app_libs/includes/iir_filter.h"
#include "config.h"

#include <string.h>

void interrupts_remap(void);
void peripheral_init(void);
void interrupts_enable_periph_ints(void);
void InitControllers(void);
interrupt void  adc1_isr( void );


#ifdef _FLASH
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadSize;
extern Uint16 RamfuncsRunStart;
#endif

//#define TESTE_HRPWM

Uint16 FlagCalib = 1;

float IOut = 0;


void main(void)
{
    long i;
// Step 1. Initialize System Control for Control and Analog Subsytems
// Enable Peripheral Clocks
// This example function is found in the F28M35x_SysCtrl.c file.
    InitSysCtrl();

    // Disable CPU interrupts
    DINT;
        
// If project is linked into flash, copy critical code sections to RAM.    
#ifdef _FLASH
   memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
   //memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)(&RamfuncsLoadStart - &RamfuncsLoadEnd));
   InitFlash();
#endif        


    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags are cleared.
    InitPieCtrl();

    // Disable CPU interrupts and clear all CPU interrupt flags:
    IER = 0x0000;
    IFR = 0x0000;
    
    InitControllers();

    InitPieVectTable();

    interrupts_remap();

    peripheral_init();

    interrupts_enable_periph_ints();
    
    EINT;       // Enable Global interrupt INTM
    ERTM;       // Enable Global realtime interrupt DBGM
    
    
    while(1) 
    {
        for(i=0; i<1000; i++);
        for(i=0; i<1000; i++);
        //GPIO_ACTION(GPIO28, TOGGLE);  
        
        GpioG1DataRegs.GPADAT.bit.GPIO28 = 1;
        GpioG1DataRegs.GPCDAT.bit.GPIO70 = 0; 
        //pwm_output_disable();     
        for(i=0; i<2000; i++);
        GpioG1DataRegs.GPADAT.bit.GPIO28 = 0;
        GpioG1DataRegs.GPCDAT.bit.GPIO70 = 1;
        for(i=0; i<2000; i++);
        //pwm_output_enable();
        
        #ifdef TESTE_HRPWM
        pwm_output_enable();
        teste_hrpwm();
        #endif
        
        pi_set_ref( shm_m2c_read.iref );
        shm_c2m_write.i_measured = IOut;
        
    }

}

/**
 * Habilita globalmente as interrupções dos periféricos desejados
 */ 
void interrupts_enable_periph_ints(void)
{
    // Step 5. User specific code, enable interrupts:
    // Enable ADCINT1 in PIE
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;  // Enable INT 1.1 in the PIE
    IER |= M_INT1;                      // Enable CPU Interrupt 1
}


/**
 * Mapeamento das rotinas que tratam as interrupções.
 */ 
void interrupts_remap(void)
{
    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.
    EALLOW; // This is needed to write to EALLOW protected register
    PieVectTable.ADCINT1 = &adc1_isr;
    EDIS;   // This is needed to disable write to EALLOW protected registers
}

/**
 * Inicialização dos periféricos.
 */ 
void peripheral_init(void)
{
    EALLOW;
    GpioG1CtrlRegs.GPBDIR.bit.GPIO32  = 1;  // Set as output
    GpioG1CtrlRegs.GPBMUX1.bit.GPIO32 = 3;  // Select EPWM1SOCA as driving source
    GpioG1CtrlRegs.GPBDIR.bit.GPIO33  = 1;  // Set as output
    GpioG1CtrlRegs.GPBMUX1.bit.GPIO33 = 3;  // Select EPWM1SOCB as driving source
    EDIS;    
    
    gpio_config();
    
    ConfigAdc1();
    
    shared_mem_init();
    
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    pwm_config();
    
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
    
}

void InitControllers(void)
{
    adc_config_constants();
    pi_controller_init(F_SAMP);
    iir_filters_init();
}

#define BUF_LEN 200
float inVectFilt[BUF_LEN];
float inVectPu[BUF_LEN];
float piOutVect[BUF_LEN];
int index = 0;
int count_int = 20;
float i_pu = 0.5;
void rotina_controle(Uint16 inRaw)
{
    int i;
    float duty;
    float in = ((int)inRaw - (int)AdcOffset) * AdcGainPU;
    float in_filtered;
    IOut = in;
    


    GPIO_ACTION(GPIO22, SET);
    //for(i=0; i<count_int; i++);
    in_filtered = iir( in, &iir_in );
//    inVectPu[index] = in;
//    inVectFilt[index] = in_filtered;
    
    i_pu = pi_controller(in,  &pi_io);
    //i_pu = pi_controller(in_filtered,  &pi_io);
    duty = 0.5 + (i_pu / 2.0);
    duty = 0.6;
    pwm_set_duty(duty, 1);
    pwm_set_duty(duty, 2);
//    piOutVect[index] = duty;

    GPIO_ACTION(GPIO22, CLEAR);
    
    index++;
    if(index == BUF_LEN)
        index = 0;
}


interrupt void  adc1_isr( void )
{
    GPIO_ACTION(GPIO20, SET);
    
    #ifndef TESTE_HRPWM
    if(FlagCalib == 1) {
        FlagCalib = adc_calib( Adc1Result.ADCRESULT1 );
        if(FlagCalib == 0)
            pwm_output_enable();
    }
    else 
        rotina_controle( Adc1Result.ADCRESULT1 );
        ;
    #endif
        
    
    Adc1Regs.ADCINTFLGCLR.bit.ADCINT1 = 1;  //Clear ADCINT1 flag reinitialize for next SOC    
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; // Acknowledge interrupt to PIE
   
    GPIO_ACTION(GPIO20, CLEAR);

    return;
}




 