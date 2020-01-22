################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/Temp/CCS/ticcs/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Temp/CCS/workspace_v9_2/pedestal_tirtos_ccs" --include_path="C:/Temp/CCS/workspace_v9_2/pedestal_tirtos_ccs/Debug" --include_path="C:/ti/simplelink_msp432e4_sdk_3_30_00_22/source/ti/net/bsd" --include_path="C:/ti/simplelink_msp432e4_sdk_3_30_00_22/source" --include_path="C:/ti/simplelink_msp432e4_sdk_3_30_00_22/source/third_party/CMSIS/Include" --include_path="C:/ti/simplelink_msp432e4_sdk_3_30_00_22/source/ti/posix/ccs" --include_path="C:/Temp/CCS/ticcs/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/include" --advice:power=none -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/Temp/CCS/workspace_v9_2/pedestal_tirtos_ccs/Debug/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-186952769:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-186952769-inproc

build-186952769-inproc: ../tcpecho.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/Temp/CCS/ticcs/ccs/utils/sysconfig/sysconfig_cli.bat" -s "C:/ti/simplelink_msp432e4_sdk_3_30_00_22/.metadata/product.json" -o "syscfg" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_drivers_config.c: build-186952769 ../tcpecho.syscfg
syscfg/ti_drivers_config.h: build-186952769
syscfg/ti_ndk_config.c: build-186952769
syscfg/ti_net_config.c: build-186952769
syscfg/syscfg_c.rov.xs: build-186952769
syscfg/: build-186952769

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/Temp/CCS/ticcs/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Temp/CCS/workspace_v9_2/pedestal_tirtos_ccs" --include_path="C:/Temp/CCS/workspace_v9_2/pedestal_tirtos_ccs/Debug" --include_path="C:/ti/simplelink_msp432e4_sdk_3_30_00_22/source/ti/net/bsd" --include_path="C:/ti/simplelink_msp432e4_sdk_3_30_00_22/source" --include_path="C:/ti/simplelink_msp432e4_sdk_3_30_00_22/source/third_party/CMSIS/Include" --include_path="C:/ti/simplelink_msp432e4_sdk_3_30_00_22/source/ti/posix/ccs" --include_path="C:/Temp/CCS/ticcs/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/include" --advice:power=none -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="C:/Temp/CCS/workspace_v9_2/pedestal_tirtos_ccs/Debug/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


