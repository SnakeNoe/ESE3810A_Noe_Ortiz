################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/contrib/apps/tcpecho/tcpecho.c 

C_DEPS += \
./lwip/contrib/apps/tcpecho/tcpecho.d 

OBJS += \
./lwip/contrib/apps/tcpecho/tcpecho.o 


# Each subdirectory must supply rules for building sources it contributes
lwip/contrib/apps/tcpecho/%.o: ../lwip/contrib/apps/tcpecho/%.c lwip/contrib/apps/tcpecho/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DUSE_RTOS=1 -DPRINTF_ADVANCED_ENABLE=1 -DFRDM_K64F -DFREEDOM -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 -DSERIAL_PORT_TYPE_UART=1 -DSDK_OS_FREE_RTOS -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\ESE3810A_Noe_Ortiz\frdmk64f_lwip_tcp\source" -I"C:\ESE3810A_Noe_Ortiz\frdmk64f_lwip_tcp\mdio" -I"C:\ESE3810A_Noe_Ortiz\frdmk64f_lwip_tcp\phy" -I"C:\ESE3810A_Noe_Ortiz\frdmk64f_lwip_tcp\lwip\contrib\apps\tcpecho" -I"C:\ESE3810A_Noe_Ortiz\frdmk64f_lwip_tcp\lwip\port" -I"C:\ESE3810A_Noe_Ortiz\frdmk64f_lwip_tcp\lwip\src" -I"C:\ESE3810A_Noe_Ortiz\frdmk64f_lwip_tcp\lwip\src\include" -I"C:\ESE3810A_Noe_Ortiz\frdmk64f_lwip_tcp\drivers" -I"C:\ESE3810A_Noe_Ortiz\frdmk64f_lwip_tcp\utilities" -I"C:\ESE3810A_Noe_Ortiz\frdmk64f_lwip_tcp\device" -I"C:\ESE3810A_Noe_Ortiz\frdmk64f_lwip_tcp\component\uart" -I"C:\ESE3810A_Noe_Ortiz\frdmk64f_lwip_tcp\component\serial_manager" -I"C:\ESE3810A_Noe_Ortiz\frdmk64f_lwip_tcp\component\lists" -I"C:\ESE3810A_Noe_Ortiz\frdmk64f_lwip_tcp\CMSIS" -I"C:\ESE3810A_Noe_Ortiz\frdmk64f_lwip_tcp\freertos\freertos_kernel\include" -I"C:\ESE3810A_Noe_Ortiz\frdmk64f_lwip_tcp\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\ESE3810A_Noe_Ortiz\frdmk64f_lwip_tcp\board" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lwip-2f-contrib-2f-apps-2f-tcpecho

clean-lwip-2f-contrib-2f-apps-2f-tcpecho:
	-$(RM) ./lwip/contrib/apps/tcpecho/tcpecho.d ./lwip/contrib/apps/tcpecho/tcpecho.o

.PHONY: clean-lwip-2f-contrib-2f-apps-2f-tcpecho

