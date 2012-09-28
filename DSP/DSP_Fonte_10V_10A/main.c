/**
 *  Código para DSP F28M35 para controle da fonte 10V-10A
 * 
 *  Criado por: Jakson Bonaldo - jbonaldo@gmail.com
 *  Data:       22/05/2012
 */

#include "DSP28x_Project.h"     // F28M35x Headerfile Include File
#include "app_libs/includes/gpio_config.h"
#include "app_libs/includes/pwm_config.h"
#include "app_libs/includes/adc.h"
#include "app_libs/includes/ad7634.h"
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
interrupt void cpu_timer2_isr(void);
interrupt void Mcbsp_RxINTA_ISR(void);
interrupt void xint2_isr(void);
interrupt void epwm1_timer_isr(void);
void swi(void);
void rotina_controle(float in);

#ifdef _FLASH
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadSize;
extern Uint16 RamfuncsRunStart;
#endif

#pragma CODE_SECTION(xint2_isr, "ramfuncs");
#pragma CODE_SECTION(Mcbsp_RxINTA_ISR, "ramfuncs");
#pragma CODE_SECTION(epwm1_timer_isr, "ramfuncs");
#pragma CODE_SECTION(rotina_controle, "ramfuncs");
#pragma CODE_SECTION(swi, "ramfuncs");

//#define TESTE_HRPWM

Uint16 FlagCalib    = 1;
Uint16 FlagAq       = 0;
long AdcAq = 0;
float   Iin;
float   Iin_shadow;
Uint16  IinCount = 0;


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
        
    /*    
        for(i=0; i<1000; i++);
        for(i=0; i<1000; i++);
        //GPIO_ACTION(GPIO28, TOGGLE);  
        
        //GpioG1DataRegs.GPADAT.bit.GPIO28 = 1;
        GpioG1DataRegs.GPCDAT.bit.GPIO70 = 0; 
        //pwm_output_disable();     
        for(i=0; i<2000; i++);
        //GpioG1DataRegs.GPADAT.bit.GPIO28 = 0;
        GpioG1DataRegs.GPCDAT.bit.GPIO70 = 1;
        for(i=0; i<2000; i++);
        //pwm_output_enable();
   */     
        #ifdef TESTE_HRPWM
        pwm_output_enable();
        teste_hrpwm();
        #endif
        
        //if( FlagAq )
        //{
        //    swi();
        //}
        
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
    //PieCtrlRegs.PIEIER1.bit.INTx1 = 1;  // Enable INT 1.1 in the PIE
    // Enable TINT0 in the PIE: Group 1 interrupt 7
    //PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    
    PieCtrlRegs.PIEIER6.bit.INTx5 = 1;  //MCBSP
    PieCtrlRegs.PIEIER1.bit.INTx5 = 1;  //XINT2
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;  //ePWM1
    IER = M_INT6;
    IER |= M_INT1;
    IER |= M_INT3;
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
    PieVectTable.TINT2 = &cpu_timer2_isr;
    PieVectTable.MRINTA = &Mcbsp_RxINTA_ISR; //McBsp rx int;
    PieVectTable.XINT2 = &xint2_isr;
    PieVectTable.EPWM1_INT =  &epwm1_timer_isr;
    EDIS;   // This is needed to disable write to EALLOW protected registers
}

/**
 * Inicialização dos periféricos.
 */ 
void peripheral_init(void)
{
    gpio_config();
    
    //ConfigAdc1();
    
    InitCpuTimers();  // For this example, only initialize the Cpu Timers
    //ConfigCpuTimer(&CpuTimer2, 150, 20);
    //CpuTimer2Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit
   
    ad7634_config();
    
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
float duty_force = 0.6;
Uint16 flag_force = 0;
void rotina_controle(float in)
{
    
    
    int i;
    float duty;
    float in_filtered;
    
    float temp;
    static int cnt = 0;
    
    IOut = in;
      
    in_filtered = iir_cascade( in, &iir_in );
    //in_filtered = iir( temp, &iir_in );
    //inVectPu[index] = in;
    //inVectFilt[index] = in_filtered;
    
    pwm_set_duty( in_filtered * 0.5 + 0.5, 3 );
    
    //i_pu = pi_controller(in,  &pi_io);
    i_pu = pi_controller(in_filtered,  &pi_io);
    duty = 0.5 + (i_pu * 0.5);
    if(flag_force == 1)
        duty = duty_force;
    pwm_set_duty(duty, 1);
    pwm_set_duty(duty, 2);
    //piOutVect[index] = duty;

    index++;
    if(index == BUF_LEN)
        index = 0;
        
}

float duty_dac_pwm;
float adc_resolution = 1.0 / 0x00040000;
int dummy_count;
int temp_comp = 1;
long vect_in[128];
int i;
void swi(void)
{
    float in;
    
    GPIO_ACTION(GPIO28, SET);

    FlagAq = 0;
    vect_in[i] = AdcAq;
    if(++i >= 128)   i = 0;
    
    
    Iin = Iin_shadow / IinCount;
    IinCount = 0;
    Iin_shadow = 0;
    //in = ((Int32)AdcAq - (Int32)AdcOffset) * AdcGainPU; 
    //Iin = in;
    
    
    //if(cnt++ > 100)
    //    temp = 0.5;
    //else
    //    temp = -0.5;
    //if(cnt > 200) cnt = 0;
    //pwm_set_duty(temp, 4);
    
    duty_dac_pwm = (Iin + 1.0) * 0.5;
    
    pwm_set_duty(duty_dac_pwm, 4);
    
    rotina_controle( Iin );
    
    
    
    GPIO_ACTION(GPIO28, CLEAR);
}


interrupt void Mcbsp_RxINTA_ISR(void)
{
    GPIO_ACTION(GPIO10, SET);

    AdcAq = McbspaRegs.DRR2.all;
    AdcAq = (AdcAq << 16) | McbspaRegs.DRR1.all;
    AdcAq = (AdcAq >> 2) & 0x3FFFF;
    
    Iin_shadow += ((Int32)AdcAq - (Int32)AdcOffset) * AdcGainPU; 
    IinCount++;
    FlagAq = 1;
    
    GPIO_ACTION(GPIO10, CLEAR);
    
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP6;
}

    
float temp;
int cnt = 0;
float in_filtered = 0;
interrupt void xint2_isr(void)
{
    //GPIO_ACTION(GPIO10, SET);
    
    McbspaRegs.DXR2.all = 0xfff;
    McbspaRegs.DXR1.all = 0xf;
    
/*
    in_filtered = iir( temp, &iir_in );
    pwm_set_duty( in_filtered * 0.5 + 0.5, 3 );
*/    
    
    
    //GPIO_ACTION(GPIO10, CLEAR);
    
    // Acknowledge this interrupt to get more from group 1
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1; 
}

interrupt void epwm1_timer_isr(void)
{
    EINT;
    #ifndef TESTE_HRPWM
    if(FlagCalib == 1) {
        FlagCalib = adc_calib( AdcAq );
        if(FlagCalib == 0)
            pwm_output_enable();
    }
    else 
        swi();
        ;
    #endif

    // Clear INT flag for this timer
    EPwm1Regs.ETCLR.bit.INT = 1;
    // Acknowledge this interrupt to receive more interrupts from group 3
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}














































interrupt void  adc1_isr( void )
{
    GPIO_ACTION(GPIO28, SET);
    
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
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1; // Acknowledge interrupt to PIE
   
    GPIO_ACTION(GPIO28, CLEAR);

    return;
}


interrupt void cpu_timer2_isr(void)
{
    GPIO_ACTION(GPIO28, SET);
    GPIO_ACTION(GPIO10, CLEAR);
    
    GPIO_ACTION(GPIO10, SET);
   
    //while( GpioG1DataRegs.GPADAT.bit.GPIO20 == temp_comp);
    
/*    McbspaRegs.DXR2.all = 0xfff;
    McbspaRegs.DXR1.all = 0xf;
    
    duty_dac_pwm = (float)dummy * adc_resolution;
    pwm_set_duty(duty_dac_pwm, 4);
*/    
    // Acknowledge this interrupt to receive more interrupts from group 1
    GPIO_ACTION(GPIO28, CLEAR);
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1;
    
}
