################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/R16529/r16529_fmc.c 

OBJS += \
./Core/R16529/r16529_fmc.o 

C_DEPS += \
./Core/R16529/r16529_fmc.d 


# Each subdirectory must supply rules for building sources it contributes
Core/R16529/%.o Core/R16529/%.su Core/R16529/%.cyclo: ../Core/R16529/%.c Core/R16529/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Core/R16529 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-R16529

clean-Core-2f-R16529:
	-$(RM) ./Core/R16529/r16529_fmc.cyclo ./Core/R16529/r16529_fmc.d ./Core/R16529/r16529_fmc.o ./Core/R16529/r16529_fmc.su

.PHONY: clean-Core-2f-R16529

