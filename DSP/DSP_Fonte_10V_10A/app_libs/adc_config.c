/**
 * Arquivo de configuração dos PWMs
 * 
 */
 
 #include "DSP28x_Project.h"
 #include "F28M35x_EPwm_defines.h"  
 #include "includes/adc_config.h"
 #include "../config.h"

#define CALIB_SAMPLES               1000
#define ADC_MAX_VALUE               4096
#define ADC_MAX_VOLTAGE             3.3
#define CONDITIONING_MAX_VOLTAGE    2.9
#define TRANSDUCER_MAX_VOLTAGE_PP   20   
#define PLANT_MAX_VOLTAGE_PP        20//24
#define NOMINAL_VALUE               20   

Uint16 AdcOffset = 0;
float AdcGainPU = 0;

void ConfigAdc1( void )
{
    // Inicializa ADC (Power On e Enable)
    extern void DSP28x_usDelay(Uint32 Count);
    #define ADC_usDELAY  1000L

    // To powerup the ADC1 the ADC1ENCLK bit should be set first to enable
    // clocks, followed by powering up the bandgap, reference circuitry, and
    // ADC1 core.
    // Before the first conversion is performed a 5ms delay must be observed
    // after power up to give all analog circuits time to power up and settle

    // Please note that for the delay function below to operate correctly the
    // CPU_RATE define statement in the F28M35xA_Examples.h file must
    // contain the correct CPU clock period in nanoseconds.
    EALLOW;
    Adc1Regs.ADCCTL1.bit.ADCBGPWD  = 1;      // Power ADC1 BG
    Adc1Regs.ADCCTL1.bit.ADCREFPWD = 1;      // Power reference
    Adc1Regs.ADCCTL1.bit.ADCPWDN   = 1;      // Power ADC1
    Adc1Regs.ADCCTL1.bit.ADCENABLE = 1;      // Enable ADC1
    Adc1Regs.ADCCTL1.bit.ADCREFSEL = 0;      // Select interal BG
    EDIS;

    DELAY_US(ADC_usDELAY);         // Delay before converting ADC1 channels
    
    // Configure ADC
    EALLOW;
    
    Adc1Regs.ADCCTL1.bit.ADCENABLE = 1;
    
    Adc1Regs.ADCCTL2.bit.ADCNONOVERLAP = 1;     // Set ADC to non-overlap mode
    
    Adc1Regs.ADCCTL1.bit.INTPULSEPOS   = 1;     // EOC trips after conversion result is latched
    Adc1Regs.INTSEL1N2.bit.INT1E       = 1;     // Enabled ADCINT1
    Adc1Regs.INTSEL1N2.bit.INT1CONT    = 0;     // Disable ADCINT1 Continuous mode
    Adc1Regs.INTSEL1N2.bit.INT1SEL     = 1;     // setup EOC1 to trigger ADCINT1 (a segunda conversao dispara a interrupcao do ADC)
                                                //0 -> EOC0; 1 -> EOC1 (final da conv. do canal B)
 
    //Para este caso, será utilizada conversao simultanea,
    //Entao quando o segundo canal (impar) for efetivamente convertido
    //Ocorrerá a interrupcao.
    
    Adc1Regs.ADCSAMPLEMODE.bit.SIMULEN0 = 1;
 
    // Select ADC input channels                                              
    Adc1Regs.ADCSOC0CTL.bit.CHSEL      = 0;     // set SOC0 channel select to ADC1A0
                                                //Se for modo simultaneo, o canal ADC1B0 também será convertido
    //Adc1Regs.ADCSOC1CTL.bit.CHSEL      = 2;   // set SOC1 channel select to ADC1A2
    
    // Selecing triggers for SOCs
    AnalogSysctrlRegs.TRIG1SEL.all     = 5;     // Assigning EPWM1SOCA to TRIGGER 5 of analog subsystem
    //AnalogSysctrlRegs.TRIG2SEL.all     = 9;     // Assigning EPWM1SOCB to TRIGGER 6 of analog subsystem
    
    Adc1Regs.ADCSOC0CTL.bit.TRIGSEL    = 5;     // Fonte do trigger = TRIG1SEL
    //Adc1Regs.ADCSOC1CTL.bit.TRIGSEL    = 6;     // Fonte do trigger = TRIG2SEL
    
    // Set S/H window of 7 ADC clock cycles                                            
    Adc1Regs.ADCSOC0CTL.bit.ACQPS      = 6;     // S/H = ACQPS + 1     
   //Adc1Regs.ADCSOC1CTL.bit.ACQPS      = 6;     
   
   Adc1Regs.ADCINTFLGCLR.all = 0xFFFF;          //Limpa todas as interrupções pendentes do ADC
   Adc1Regs.ADCINTOVFCLR.all = 0xFFFF;
   Adc1Regs.ADCSOCOVFCLR1.all = 0xFFFF;
   
    
    EDIS;
}

int adc_calib(unsigned int in)
{
    static Uint16 count = 0;
    static long acc = 0;

    acc += in;
    
    count++;
    if(count == CALIB_SAMPLES)
    {
        AdcOffset = acc / CALIB_SAMPLES;
        return 0;
    }
    
    return 1;
}

void adc_config_constants(void)
{
    float kTransducer = (float) PLANT_MAX_VOLTAGE_PP/TRANSDUCER_MAX_VOLTAGE_PP;
    float kConditioningCircuit = (float) TRANSDUCER_MAX_VOLTAGE_PP/CONDITIONING_MAX_VOLTAGE;
    //float kAdjustConditioningDsp = (float) CONDITIONING_MAX_VOLTAGE / ADC_MAX_VOLTAGE;
    float AdcRealResolution = ((float) CONDITIONING_MAX_VOLTAGE / ADC_MAX_VOLTAGE) * ADC_MAX_VALUE;
    float kAdc = (float) ADC_MAX_VOLTAGE / ADC_MAX_VALUE;
    
    float kConv = kTransducer * kConditioningCircuit * kAdc;
    //float kConv = kTransducer * kAdjustConditioningDsp * kAdc;
    
    AdcGainPU = kConv / NOMINAL_VALUE;
    AdcGainPU = 2.0 * AdcGainPU; //(2.0 utilizado para adaptar o valor devido a extração do nível médio)
}

