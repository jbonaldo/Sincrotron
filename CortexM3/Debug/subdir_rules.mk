################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
F28M35x_generic_M3_FLASH.out: ../F28M35x_generic_M3_FLASH.cmd $(GEN_CMDS)
	@echo 'Building file: $<'
	@echo 'Invoking: Linker'
	"C:/Program Files (x86)/Texas Instruments/TMS470 Code Generation Tools 4.9.0/bin/cl470" -mv7M3 -g --gcc --define=ccs --diag_warning=225 -me --abi=eabi --code_state=16 --ual -z -m"CortexM3.map" --stack_size=4096 --heap_size=4096 --warn_sections -i"C:/Program Files (x86)/Texas Instruments/TMS470 Code Generation Tools 4.9.0/lib" -i"C:/Arquivos/Doutorado/Sincrotron/Implementacao/CortexM3/.." -i"C:/Program Files (x86)/Texas Instruments/TMS470 Code Generation Tools 4.9.0/include" --reread_libs --rom_model -o "$@" "$<" "../F28M35x_generic_M3_FLASH.cmd" "../driverlib.lib"
	@echo 'Finished building: $<'
	@echo ' '

startup_ccs.obj: ../startup_ccs.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/TMS470 Code Generation Tools 4.9.0/bin/cl470" -mv7M3 -g --gcc --define=ccs --include_path="C:/Program Files (x86)/Texas Instruments/TMS470 Code Generation Tools 4.9.0/include" --include_path="C:/Arquivos/Doutorado/Sincrotron/Implementacao/CortexM3/Servidor/uip-1.0/apps" --include_path="C:/TI/controlSUITE/device_support/f28m35x/v130/MWare/" --include_path="C:/Arquivos/Doutorado/Sincrotron/Implementacao/CortexM3/Servidor/uip-1.0" --include_path="C:/Arquivos/Doutorado/Sincrotron/Implementacao/CortexM3/Servidor/includes" --include_path="C:/Arquivos/Doutorado/Sincrotron/Implementacao/CortexM3/Servidor/uip-1.0/uip" --diag_warning=225 -me --abi=eabi --code_state=16 --ual --preproc_with_compile --preproc_dependency="startup_ccs.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


