
#include "inc/hw_sysctl.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/ipc.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "driverlib/cpu.c"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/ram.h"

#include <string.h>

extern unsigned long RamfuncsLoadStart;
extern unsigned long RamfuncsRunStart;
extern unsigned long RamfuncsLoadSize;

#define M3_MASTER 0
#define C28_MASTER 1


void c28_config(void)
{
    int i = 0;
    
    // Allow writes to protected registers.
    HWREG(SYSCTL_MWRALLOW) = 0xA5A5A5A5;

    // Tells M3 Core the vector table is at the beginning of C0 now.
    HWREG(NVIC_VTABLE) = 0x20005000;

/*
    //Configura Arm 100 MHz e DSP 100 MHz
    SysCtlClockConfigSet(SYSCTL_USE_PLL | (SYSCTL_SPLLIMULT_M & 0xA) |
                         SYSCTL_SYSDIV_1 | SYSCTL_M3SSDIV_1 |
                         SYSCTL_XCLKDIV_1);
*/

/*    // Setup main clock tree for 75MHz - M3 and 150MHz - C28x
    SysCtlClockConfigSet(SYSCTL_SYSDIV_1 | SYSCTL_M3SSDIV_2 | SYSCTL_USE_PLL |
                         (SYSCTL_SPLLIMULT_M & 0x0F));
*/

    // Sets up PLL, M3 running at 75MHz and C28 running at 150MHz
    SysCtlClockConfigSet(SYSCTL_USE_PLL | (SYSCTL_SPLLIMULT_M & 0xF) |
                         SYSCTL_SYSDIV_1 | SYSCTL_M3SSDIV_2 |
                         SYSCTL_XCLKDIV_4);                                                                       
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);    


    FlashInit();      
    
    // assign S0 and S1 of the shared ram for use by the c28
    // Details of how c28 uses these memory sections is defined
    // in the c28 linker file.(28M35H52C1_RAM_lnk.cmd)
    RAMMReqSharedMemAccess((S0_ACCESS | S1_ACCESS),C28_MASTER);                   
                      
    IPCMtoCBootControlSystem(CBROM_MTOC_BOOTMODE_BOOT_FROM_FLASH); 

    // Enable clock supply for the following peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ETH);
    

    // Enable Pullups on EPWM(1-8)A/B capable pins
    GPIOPadConfigSet(GPIO_PORTA_BASE, 0xFF, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTB_BASE, 0xFF, GPIO_PIN_TYPE_STD_WPU);

    // Give C28 control of all GPIOs
    GPIOPinConfigureCoreSelect(GPIO_PORTA_BASE, 0xFF, GPIO_PIN_C_CORE_SELECT);
    GPIOPinConfigureCoreSelect(GPIO_PORTB_BASE, 0xFF, GPIO_PIN_C_CORE_SELECT);
    //GPIOPinConfigureCoreSelect(GPIO_PORTC_BASE, 0xFF & (~GPIO_PIN_7), GPIO_PIN_C_CORE_SELECT);
    GPIOPinConfigureCoreSelect(GPIO_PORTD_BASE, 0xFF, GPIO_PIN_C_CORE_SELECT);
    GPIOPinConfigureCoreSelect(GPIO_PORTE_BASE, 0xFF & (~GPIO_PIN_6), GPIO_PIN_C_CORE_SELECT); //GPIO30 - GPE6 é utilizado pela ethernet
    //GPIOPinConfigureCoreSelect(GPIO_PORTF_BASE, 0xFF, GPIO_PIN_C_CORE_SELECT);
    //GPIOPinConfigureCoreSelect(GPIO_PORTG_BASE, 0xFF, GPIO_PIN_C_CORE_SELECT);
    //GPIOPinConfigureCoreSelect(GPIO_PORTH_BASE, 0xFF, GPIO_PIN_C_CORE_SELECT);
    //GPIOPinConfigureCoreSelect(GPIO_PORTJ_BASE, 0xFF, GPIO_PIN_C_CORE_SELECT);
    
    
    // Unlock the register to change the commit register value for Port B Pin 7
    // This disables the NMI functionality on the pin and allows other muxing 
    // options to be used
    HWREG(GPIO_PORTB_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
    // Write to commit register
    HWREG(GPIO_PORTB_BASE+GPIO_O_CR) |= 0x000000FF;
    // Delay
    for (i=0;i<2000;i++){};

    //  Send boot command to allow the C28 application to begin execution
//    IPCMtoCBootControlSystem(CBROM_MTOC_BOOTMODE_BOOT_FROM_FLASH);      
    
        //Enable processor interrupts.
    IntMasterEnable(); 
}