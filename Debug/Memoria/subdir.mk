################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Memoria/Memoria.c 

OBJS += \
./Memoria/Memoria.o 

C_DEPS += \
./Memoria/Memoria.d 


# Each subdirectory must supply rules for building sources it contributes
Memoria/%.o Memoria/%.su Memoria/%.cyclo: ../Memoria/%.c Memoria/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -c -I../USB_DEVICE/App -I../OSC -I../USB_DEVICE/Target -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I../Touch -I../ILI9341 -I../Recursos -I../App -I../Memoria -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Memoria

clean-Memoria:
	-$(RM) ./Memoria/Memoria.cyclo ./Memoria/Memoria.d ./Memoria/Memoria.o ./Memoria/Memoria.su

.PHONY: clean-Memoria

