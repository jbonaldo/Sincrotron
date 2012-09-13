################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

EMPTY := 
SPACE := $(EMPTY) $(EMPTY)
CG_TOOL_ROOT := C:/Program Files (x86)/Texas Instruments/TMS470 Code Generation Tools 4.9.0

ORDERED_OBJS += \
$(GEN_CMDS_QUOTED) \
"./Servidor/httpd.obj" \
"./Servidor/enet_uip.obj" \
"./Servidor/uip-1.0/apps/dhcpc/dhcpc.obj" \
"./Servidor/uip-1.0/uip/uiplib.obj" \
"./Servidor/uip-1.0/uip/uip_timer.obj" \
"./Servidor/uip-1.0/uip/uip_arp.obj" \
"./Servidor/uip-1.0/uip/uip.obj" \
"./Servidor/uip-1.0/uip/uip-fw.obj" \
"./app/shared_mem_arm.obj" \
"./app/main.obj" \
"./app/commands.obj" \
"./app/c28_config.obj" \
"./startup_ccs.obj" \
"./utils/ustdlib.obj" \
"./utils/uartstdio.obj" \
-l"rtsv7M3_T_le_eabi.lib" \
"../F28M35x_generic_M3_FLASH.cmd" \
"C:/ti/controlSUITE/device_support/f28m35x/v130/MWare/driverlib/ccs/Debug/driverlib.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include utils/subdir_vars.mk
-include app/subdir_vars.mk
-include Servidor/uip-1.0/uip/subdir_vars.mk
-include Servidor/uip-1.0/apps/dhcpc/subdir_vars.mk
-include Servidor/subdir_vars.mk
-include subdir_rules.mk
-include utils/subdir_rules.mk
-include app/subdir_rules.mk
-include Servidor/uip-1.0/uip/subdir_rules.mk
-include Servidor/uip-1.0/apps/dhcpc/subdir_rules.mk
-include Servidor/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: CortexM3.out

# Tool invocations
CortexM3.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: Linker'
	"C:/Program Files (x86)/Texas Instruments/TMS470 Code Generation Tools 4.9.0/bin/cl470" -mv7M3 -g --gcc --define=ccs --diag_warning=225 -me --abi=eabi --code_state=16 --ual -z -m"CortexM3.map" --stack_size=4096 --heap_size=4096 --warn_sections -i"C:/Program Files (x86)/Texas Instruments/TMS470 Code Generation Tools 4.9.0/lib" -i"C:/Arquivos/Doutorado/Sincrotron/Implementacao/CortexM3/.." -i"C:/Program Files (x86)/Texas Instruments/TMS470 Code Generation Tools 4.9.0/include" --reread_libs --rom_model -o "CortexM3.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(TMS470_EXECUTABLE_OUTPUTS__QTD) "CortexM3.out"
	-$(RM) ".\utils\uartstdio.pp" ".\utils\ustdlib.pp" ".\startup_ccs.pp" ".\app\c28_config.pp" ".\app\commands.pp" ".\app\main.pp" ".\app\shared_mem_arm.pp" ".\Servidor\uip-1.0\uip\uip-fw.pp" ".\Servidor\uip-1.0\uip\uip.pp" ".\Servidor\uip-1.0\uip\uip_arp.pp" ".\Servidor\uip-1.0\uip\uip_timer.pp" ".\Servidor\uip-1.0\uip\uiplib.pp" ".\Servidor\uip-1.0\apps\dhcpc\dhcpc.pp" ".\Servidor\enet_uip.pp" ".\Servidor\httpd.pp" 
	-$(RM) ".\utils\uartstdio.obj" ".\utils\ustdlib.obj" ".\startup_ccs.obj" ".\app\c28_config.obj" ".\app\commands.obj" ".\app\main.obj" ".\app\shared_mem_arm.obj" ".\Servidor\uip-1.0\uip\uip-fw.obj" ".\Servidor\uip-1.0\uip\uip.obj" ".\Servidor\uip-1.0\uip\uip_arp.obj" ".\Servidor\uip-1.0\uip\uip_timer.obj" ".\Servidor\uip-1.0\uip\uiplib.obj" ".\Servidor\uip-1.0\apps\dhcpc\dhcpc.obj" ".\Servidor\enet_uip.obj" ".\Servidor\httpd.obj" 
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
