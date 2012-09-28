#ifndef ADC_H_
#define ADC_H_

int adc_calib(unsigned long in);
void adc_config_constants(void);

extern Uint32 AdcOffset;
extern float AdcGainPU;


#endif /*ADC_H_*/
