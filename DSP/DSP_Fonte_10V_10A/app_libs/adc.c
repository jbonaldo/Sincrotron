/**
 *  Rotinas gerais do ADC
 * 
 * Utilizadas para qualquer adc;
 *
 */
 
#include "DSP28x_Project.h"
#include "includes/adc.h"

#define ADC_C28X_MAX_VALUE          4096;            
#define AD7634_MAX_VALUE            0x40000

#define CALIB_SAMPLES               1000
#define ADC_MAX_VALUE               AD7634_MAX_VALUE
#define ADC_MAX_VOLTAGE             3.3
#define CONDITIONING_MAX_VOLTAGE    2.9
#define TRANSDUCER_MAX_VOLTAGE_PP   20   
#define PLANT_MAX_VOLTAGE_PP        20//24
#define NOMINAL_VALUE               20   

Uint32 AdcOffset = 0;
float AdcGainPU = 0;

int adc_calib(unsigned long in)
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


