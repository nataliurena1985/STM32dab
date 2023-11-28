################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Aplicacion.c \
../App/Dab.c \
../App/Idioma.c \
../App/Inverter.c \
../App/Mppt.c \
../App/OSC.c \
../App/Panel\ Tactil.c 

OBJS += \
./App/Aplicacion.o \
./App/Dab.o \
./App/Idioma.o \
./App/Inverter.o \
./App/Mppt.o \
./App/OSC.o \
./App/Panel\ Tactil.o 

C_DEPS += \
./App/Aplicacion.d \
./App/Dab.d \
./App/Idioma.d \
./App/Inverter.d \
./App/Mppt.d \
./App/OSC.d \
./App/Panel\ Tactil.d 


# Each subdirectory must supply rules for building sources it contributes
App/%.o App/%.su App/%.cyclo: ../App/%.c App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F303xE -c -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../OSC -I../ILI9341 -I../Memoria -I../Recursos -I../App -I../Touch -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
App/Panel\ Tactil.o: ../App/Panel\ Tactil.c App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F303xE -c -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../OSC -I../ILI9341 -I../Memoria -I../Recursos -I../App -I../Touch -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"App/Panel Tactil.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App

clean-App:
	-$(RM) ./App/Aplicacion.cyclo ./App/Aplicacion.d ./App/Aplicacion.o ./App/Aplicacion.su ./App/Dab.cyclo ./App/Dab.d ./App/Dab.o ./App/Dab.su ./App/Idioma.cyclo ./App/Idioma.d ./App/Idioma.o ./App/Idioma.su ./App/Inverter.cyclo ./App/Inverter.d ./App/Inverter.o ./App/Inverter.su ./App/Mppt.cyclo ./App/Mppt.d ./App/Mppt.o ./App/Mppt.su ./App/OSC.cyclo ./App/OSC.d ./App/OSC.o ./App/OSC.su ./App/Panel\ Tactil.cyclo ./App/Panel\ Tactil.d ./App/Panel\ Tactil.o ./App/Panel\ Tactil.su

.PHONY: clean-App

