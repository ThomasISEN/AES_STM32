################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/aes/aes.c 

OBJS += \
./Core/Src/aes/aes.o 

C_DEPS += \
./Core/Src/aes/aes.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/aes/%.o Core/Src/aes/%.su Core/Src/aes/%.cyclo: ../Core/Src/aes/%.c Core/Src/aes/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-aes

clean-Core-2f-Src-2f-aes:
	-$(RM) ./Core/Src/aes/aes.cyclo ./Core/Src/aes/aes.d ./Core/Src/aes/aes.o ./Core/Src/aes/aes.su

.PHONY: clean-Core-2f-Src-2f-aes

