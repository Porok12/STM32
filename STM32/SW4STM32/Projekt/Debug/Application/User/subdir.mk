################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/przemek/workspace/Projekt/Src/main.c \
/home/przemek/workspace/Projekt/Src/stm32f3xx_hal_msp.c \
/home/przemek/workspace/Projekt/Src/stm32f3xx_it.c 

OBJS += \
./Application/User/main.o \
./Application/User/stm32f3xx_hal_msp.o \
./Application/User/stm32f3xx_it.o 

C_DEPS += \
./Application/User/main.d \
./Application/User/stm32f3xx_hal_msp.d \
./Application/User/stm32f3xx_it.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/main.o: /home/przemek/workspace/Projekt/Src/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F334x8 -I"/home/przemek/workspace/Projekt/Inc" -I"/home/przemek/workspace/Projekt/Drivers/STM32F3xx_HAL_Driver/Inc" -I"/home/przemek/workspace/Projekt/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"/home/przemek/workspace/Projekt/Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"/home/przemek/workspace/Projekt/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/stm32f3xx_hal_msp.o: /home/przemek/workspace/Projekt/Src/stm32f3xx_hal_msp.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F334x8 -I"/home/przemek/workspace/Projekt/Inc" -I"/home/przemek/workspace/Projekt/Drivers/STM32F3xx_HAL_Driver/Inc" -I"/home/przemek/workspace/Projekt/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"/home/przemek/workspace/Projekt/Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"/home/przemek/workspace/Projekt/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/stm32f3xx_it.o: /home/przemek/workspace/Projekt/Src/stm32f3xx_it.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F334x8 -I"/home/przemek/workspace/Projekt/Inc" -I"/home/przemek/workspace/Projekt/Drivers/STM32F3xx_HAL_Driver/Inc" -I"/home/przemek/workspace/Projekt/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"/home/przemek/workspace/Projekt/Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"/home/przemek/workspace/Projekt/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


