################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
source2/F28M35x_GlobalVariableDefs.obj: C:/Arquivos/Doutorado/Sincrotron/Implementacao/DSP/F28M35x_headers/source/F28M35x_GlobalVariableDefs.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/C2000 Code Generation Tools 6.0.1/bin/cl2000" -v28 -mt -ml -g --define="_FLASH" --include_path="C:/Program Files (x86)/Texas Instruments/C2000 Code Generation Tools 6.0.1/include" --include_path="C:/Arquivos/Doutorado/Sincrotron/Implementacao/DSP/F28M35x_headers/include" --include_path="C:/Arquivos/Doutorado/Sincrotron/Implementacao/DSP/F28M35x_common/include" --diag_warning=225 --float_support=fpu32 --vcu_support=vcu0 --preproc_with_compile --preproc_dependency="source2/F28M35x_GlobalVariableDefs.pp" --obj_directory="source2" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


