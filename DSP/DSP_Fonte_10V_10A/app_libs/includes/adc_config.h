#ifndef ADC_CONFIG_H_
#define ADC_CONFIG_H_

void ConfigAdc1( void );
int adc_calib(unsigned int in);
void adc_config_constants(void);

extern Uint16 AdcOffset;
extern float AdcGainPU;

#endif /*ADC_CONFIG_H_*/
