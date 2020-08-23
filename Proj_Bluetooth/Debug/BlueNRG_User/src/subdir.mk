################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BlueNRG_User/src/ble_app.c 

OBJS += \
./BlueNRG_User/src/ble_app.o 

C_DEPS += \
./BlueNRG_User/src/ble_app.d 


# Each subdirectory must supply rules for building sources it contributes
BlueNRG_User/src/ble_app.o: ../BlueNRG_User/src/ble_app.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Core/Inc -I../BlueNRG_User/inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/BlueNRG-MS/utils -I../Middlewares/ST/BlueNRG-MS/includes -I../Middlewares/ST/BlueNRG-MS/hci/hci_tl_patterns/Basic -I../BlueNRG-MS/Target -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"BlueNRG_User/src/ble_app.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

