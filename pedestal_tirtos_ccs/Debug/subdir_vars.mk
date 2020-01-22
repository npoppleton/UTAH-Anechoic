################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../MSP_EXP432E401Y_TIRTOS.cmd 

SYSCFG_SRCS += \
../tcpecho.syscfg 

C_SRCS += \
../main_tirtos.c \
../tcpEcho.c \
../tcpEchoHooks.c \
./syscfg/ti_drivers_config.c \
./syscfg/ti_ndk_config.c \
./syscfg/ti_net_config.c 

GEN_FILES += \
./syscfg/ti_drivers_config.c \
./syscfg/ti_ndk_config.c \
./syscfg/ti_net_config.c 

GEN_MISC_DIRS += \
./syscfg/ 

C_DEPS += \
./main_tirtos.d \
./tcpEcho.d \
./tcpEchoHooks.d \
./syscfg/ti_drivers_config.d \
./syscfg/ti_ndk_config.d \
./syscfg/ti_net_config.d 

OBJS += \
./main_tirtos.obj \
./tcpEcho.obj \
./tcpEchoHooks.obj \
./syscfg/ti_drivers_config.obj \
./syscfg/ti_ndk_config.obj \
./syscfg/ti_net_config.obj 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/syscfg_c.rov.xs 

GEN_MISC_DIRS__QUOTED += \
"syscfg\" 

OBJS__QUOTED += \
"main_tirtos.obj" \
"tcpEcho.obj" \
"tcpEchoHooks.obj" \
"syscfg\ti_drivers_config.obj" \
"syscfg\ti_ndk_config.obj" \
"syscfg\ti_net_config.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg\ti_drivers_config.h" \
"syscfg\syscfg_c.rov.xs" 

C_DEPS__QUOTED += \
"main_tirtos.d" \
"tcpEcho.d" \
"tcpEchoHooks.d" \
"syscfg\ti_drivers_config.d" \
"syscfg\ti_ndk_config.d" \
"syscfg\ti_net_config.d" 

GEN_FILES__QUOTED += \
"syscfg\ti_drivers_config.c" \
"syscfg\ti_ndk_config.c" \
"syscfg\ti_net_config.c" 

C_SRCS__QUOTED += \
"../main_tirtos.c" \
"../tcpEcho.c" \
"../tcpEchoHooks.c" \
"./syscfg/ti_drivers_config.c" \
"./syscfg/ti_ndk_config.c" \
"./syscfg/ti_net_config.c" 

SYSCFG_SRCS__QUOTED += \
"../tcpecho.syscfg" 


