################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/Temp/CCS/ticcs/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Temp/CCS/workspace_v9_2/pedestal_tirtos_ccs" --include_path="C:/Temp/CCS/workspace_v9_2/pedestal_tirtos_ccs/Debug" --include_path="C:/ti/simplelink_msp432e4_sdk_3_30_00_22/source/ti/net/bsd" --include_path="C:/ti/simplelink_msp432e4_sdk_3_30_00_22/source" --include_path="C:/ti/simplelink_msp432e4_sdk_3_30_00_22/source/third_party/CMSIS/Include" --include_path="C:/ti/simplelink_msp432e4_sdk_3_30_00_22/source/ti/posix/ccs" --include_path="C:/Temp/CCS/ticcs/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/include" --advice:power=none -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="src/$(basename $(<F)).d_raw" --include_path="C:/Temp/CCS/workspace_v9_2/pedestal_tirtos_ccs/Debug/syscfg" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


