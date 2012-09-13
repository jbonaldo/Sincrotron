################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
F28M35x_generic_wshared_C28_FLASH.out: ../F28M35x_generic_wshared_C28_FLASH.cmd $(GEN_CMDS)
	@echo 'Building file: $<'
	@echo 'Invoking: Linker'
	"C:/Program Files (x86)/Texas Instruments/C2000 Code Generation Tools 6.0.1/bin/cl2000" -v28 -mt -ml -g --define="_FLASH" --diag_warning=225 --float_support=fpu32 --vcu_support=vcu0 -z -m"DSP_Fonte_10V_10A.map" --stack_size=0x300 --warn_sections -i"C:/Program Files (x86)/Texas Instruments/C2000 Code Generation Tools 6.0.1/lib" -i"C:/Program Files (x86)/Texas Instruments/C2000 Code Generation Tools 6.0.1/include" --reread_libs --entry_point=code_start --rom_model -o "$@" "$<" "../F28M35x_Headers_nonBIOS.cmd" "../F28M35x_generic_wshared_C28_FLASH.cmd" "../SFO_TI_Build_V7_FPU.lib"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/C2000 Code Generation Tools 6.0.1/bin/cl2000" -v28 -mt -ml -g --define="_FLASH" --include_path="C:/Program Files (x86)/Texas Instruments/C2000 Code Generation Tools 6.0.1/include" --include_path="C:/Arquivos/Doutorado/Sincrotron/Implementacao/DSP/F28M35x_headers/include" --include_path="C:/Arquivos/Doutorado/Sincrotron/Implementacao/DSP/F28M35x_common/include" --diag_warning=225 --float_support=fpu32 --vcu_support=vcu0 --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


