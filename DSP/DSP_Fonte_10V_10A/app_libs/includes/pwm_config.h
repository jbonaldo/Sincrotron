#ifndef PWM_CONFIG_H_
#define PWM_CONFIG_H_


void pwm_config(void);
void InitEPwm1Gpio(void);

void pwm_output_enable(void);
void pwm_output_disable(void);
int pwm_mep_sfo_tunning(void);
void pwm_mep_sfo_init(void);
void pwm_set_duty( float duty_pu, Uint16 module);
void teste_hrpwm(void);


#define TZ_ONE_SHOT     1

#endif /*PWM_CONFIG_H_*/
