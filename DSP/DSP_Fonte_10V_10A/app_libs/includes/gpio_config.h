#ifndef GPIO_CONFIG_H_
#define GPIO_CONFIG_H_


#define OUTPUT  1
#define INPUT   0
#define GPIO    0

#define GPIO_ACTION(pino, funcao)   GpioG1DataRegs.GPA##funcao##.bit.##pino = 1

void gpio_config(void);

#endif /*GPIO_CONFIG_H_*/
