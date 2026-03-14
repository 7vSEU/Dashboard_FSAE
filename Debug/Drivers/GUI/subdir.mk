################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/GUI/GUI.c 

OBJS += \
./Drivers/GUI/GUI.o 

C_DEPS += \
./Drivers/GUI/GUI.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/GUI/%.o Drivers/GUI/%.su Drivers/GUI/%.cyclo: ../Drivers/GUI/%.c Drivers/GUI/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"D:/Attach/Documents/STM32/STM32CubeIDE/workspace_1.19.0/Dashboard_H743/Drivers/ST7796" -I"D:/Attach/Documents/STM32/STM32CubeIDE/workspace_1.19.0/Dashboard_H743/Drivers/GUI" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-GUI

clean-Drivers-2f-GUI:
	-$(RM) ./Drivers/GUI/GUI.cyclo ./Drivers/GUI/GUI.d ./Drivers/GUI/GUI.o ./Drivers/GUI/GUI.su

.PHONY: clean-Drivers-2f-GUI

