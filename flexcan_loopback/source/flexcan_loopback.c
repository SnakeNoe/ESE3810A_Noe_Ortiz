/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_flexcan.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_CAN            CAN0
#define EXAMPLE_CAN_CLK_SOURCE (kFLEXCAN_ClkSrc1)
#define EXAMPLE_CAN_CLK_FREQ   CLOCK_GetFreq(kCLOCK_BusClk)
/* Set USE_IMPROVED_TIMING_CONFIG macro to use api to calculates the improved CAN / CAN FD timing values. */
#define USE_IMPROVED_TIMING_CONFIG (1U)
#define EXAMPLE_FLEXCAN_IRQn       CAN0_ORed_Message_buffer_IRQn
#define EXAMPLE_FLEXCAN_IRQHandler CAN0_ORed_Message_buffer_IRQHandler
#define RX_MESSAGE_BUFFER_0x10     (9)
#define RX_MESSAGE_BUFFER_0x11     (10)
#define TX_MESSAGE_BUFFER_NUM      (8)
#define DLC                        (8)
#define LED_ON					   	1
#define LED_OFF						0

/* Fix MISRA_C-2012 Rule 17.7. */
#define LOG_INFO (void)PRINTF
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void changeEndianess(uint32_t *word0, uint32_t *word1);
void delay(uint32_t seconds);

/*******************************************************************************
 * Global variables
 ******************************************************************************/
flexcan_frame_t txFrame, rxFrame;
volatile uint8_t id = 0;

/*******************************************************************************
 * Interruptions
 ******************************************************************************/
void BOARD_SW3_IRQ_HANDLER(void){
    GPIO_PortClearInterruptFlags(BOARD_SW3_GPIO, 1U << BOARD_SW3_GPIO_PIN);

    SDK_ISR_EXIT_BARRIER;
}

void BOARD_SW2_IRQ_HANDLER(void){
    GPIO_PortClearInterruptFlags(BOARD_SW2_GPIO, 1U << BOARD_SW2_GPIO_PIN);

    SDK_ISR_EXIT_BARRIER;
}


/*******************************************************************************
 * Code
 ******************************************************************************/

void EXAMPLE_FLEXCAN_IRQHandler(void)
{
    uint32_t flag = 1U;
    /* If new data arrived. */
    if (0U != FLEXCAN_GetMbStatusFlags(EXAMPLE_CAN, flag << RX_MESSAGE_BUFFER_0x10))
    {
    	id = 0x10;
        FLEXCAN_ClearMbStatusFlags(EXAMPLE_CAN, flag << RX_MESSAGE_BUFFER_0x10);
        (void)FLEXCAN_ReadRxMb(EXAMPLE_CAN, RX_MESSAGE_BUFFER_0x10, &rxFrame);
    }
    if (0U != FLEXCAN_GetMbStatusFlags(EXAMPLE_CAN, flag << RX_MESSAGE_BUFFER_0x11))
	{
    	id = 0x11;
		FLEXCAN_ClearMbStatusFlags(EXAMPLE_CAN, flag << RX_MESSAGE_BUFFER_0x11);
		(void)FLEXCAN_ReadRxMb(EXAMPLE_CAN, RX_MESSAGE_BUFFER_0x11, &rxFrame);
	}
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    flexcan_config_t flexcanConfig;
    flexcan_rx_mb_config_t mbConfig;
    flexcan_rx_mb_config_t mbConfig1;
    uint32_t flag = 1U;

	/* Define the init structure for the input switch pin */
	gpio_pin_config_t sw_config = {
		kGPIO_DigitalInput,
		0,
	};

	gpio_pin_config_t led_config = {
		kGPIO_DigitalOutput,
		0,
	};

    /* Initialize board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Init SW3 GPIO */
	PORT_SetPinInterruptConfig(BOARD_SW3_PORT, BOARD_SW3_GPIO_PIN, kPORT_InterruptFallingEdge);
	EnableIRQ(BOARD_SW3_IRQ);
	GPIO_PinInit(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN, &sw_config);

	/* Init SW2 GPIO */
  	PORT_SetPinInterruptConfig(BOARD_SW2_PORT, BOARD_SW2_GPIO_PIN, kPORT_InterruptFallingEdge);
	EnableIRQ(BOARD_SW2_IRQ);
	GPIO_PinInit(BOARD_SW2_GPIO, BOARD_SW2_GPIO_PIN, &sw_config);

    /* Init output LEDs GPIO */
	GPIO_PinInit(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, &led_config);
	GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, &led_config);
	GPIO_PinInit(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, &led_config);

	/* Turn off all LEDs */
	GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, 1);
	GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, 1);
	GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, 1);

    LOG_INFO("\r\n==FlexCAN loopback functional example -- Start.==\r\n\r\n");

    /* Init FlexCAN module. */
    /*
     * flexcanConfig.clkSrc                 = kFLEXCAN_ClkSrc0;
     * flexcanConfig.baudRate               = 1000000U;
     * flexcanConfig.baudRateFD             = 2000000U;
     * flexcanConfig.maxMbNum               = 16;
     * flexcanConfig.enableLoopBack         = false;
     * flexcanConfig.enableSelfWakeup       = false;
     * flexcanConfig.enableIndividMask      = false;
     * flexcanConfig.disableSelfReception   = false;
     * flexcanConfig.enableListenOnlyMode   = false;
     * flexcanConfig.enableDoze             = false;
     */
    FLEXCAN_GetDefaultConfig(&flexcanConfig);

    flexcanConfig.clkSrc = EXAMPLE_CAN_CLK_SOURCE;

    //flexcanConfig.enableLoopBack = true;
    flexcanConfig.baudRate             = 125000U;
    flexcan_timing_config_t timing_config;
    memset(&timing_config, 0, sizeof(flexcan_timing_config_t));
    if (FLEXCAN_CalculateImprovedTimingValues(EXAMPLE_CAN, flexcanConfig.baudRate, EXAMPLE_CAN_CLK_FREQ,
                                              &timing_config))
    {
        /* Update the improved timing configuration*/
        memcpy(&(flexcanConfig.timingConfig), &timing_config, sizeof(flexcan_timing_config_t));
    }
    else
    {
        LOG_INFO("No found Improved Timing Configuration. Just used default configuration\r\n\r\n");
    }

    FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);

    /* Setup Rx Message Buffer. */
    mbConfig.format = kFLEXCAN_FrameFormatStandard;
    mbConfig.type   = kFLEXCAN_FrameTypeData;
    mbConfig.id     = FLEXCAN_ID_STD(0x10);

    FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_0x10, &mbConfig, true);

    /* Setup Rx Message Buffer. */
	mbConfig1.format = kFLEXCAN_FrameFormatStandard;
	mbConfig1.type   = kFLEXCAN_FrameTypeData;
	mbConfig1.id     = FLEXCAN_ID_STD(0x11);

	FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_0x11, &mbConfig1, true);

    /* Setup Tx Message Buffer. */
    FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);

    /* Enable Rx Message Buffer interrupt. */
    FLEXCAN_EnableMbInterrupts(EXAMPLE_CAN, flag << RX_MESSAGE_BUFFER_0x10);
    FLEXCAN_EnableMbInterrupts(EXAMPLE_CAN, flag << RX_MESSAGE_BUFFER_0x11);
    (void)EnableIRQ(EXAMPLE_FLEXCAN_IRQn);

    /* Prepare Tx Frame for sending. */
    txFrame.format = (uint8_t)kFLEXCAN_FrameFormatStandard;
    txFrame.type   = (uint8_t)kFLEXCAN_FrameTypeData;
    txFrame.id     = FLEXCAN_ID_STD(0x20);
    txFrame.length = (uint8_t)DLC;
    txFrame.dataWord0 = CAN_WORD0_DATA_BYTE_0(0x11) | CAN_WORD0_DATA_BYTE_1(0x22) | CAN_WORD0_DATA_BYTE_2(0x33) |
                        CAN_WORD0_DATA_BYTE_3(0x44);
    txFrame.dataWord1 = CAN_WORD1_DATA_BYTE_4(0x55) | CAN_WORD1_DATA_BYTE_5(0x66) | CAN_WORD1_DATA_BYTE_6(0x77) |
                        CAN_WORD1_DATA_BYTE_7(0x88);

    LOG_INFO("Send message from MB%d to MB%d\r\n", TX_MESSAGE_BUFFER_NUM, RX_MESSAGE_BUFFER_0x10);
    LOG_INFO("tx word0 = 0x%x\r\n", txFrame.dataWord0);
    LOG_INFO("tx word1 = 0x%x\r\n", txFrame.dataWord1);

    /* Send data through Tx Message Buffer using polling function. */
    (void)FLEXCAN_TransferSendBlocking(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, &txFrame);

    while(1){
    	/* led0 signal */
		if(id == 0x10){
			changeEndianess(&rxFrame.dataWord0, &rxFrame.dataWord1);
			PRINTF("\r\nReceived message leds_values (0x10)\r\n");
			PRINTF("led0 = 0x%x\r\n", rxFrame.dataByte3);
			if(rxFrame.dataWord0 == LED_ON){
				GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, 0);
			}
			else if(rxFrame.dataWord0 == LED_OFF){
				GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, 1);
			}
			id = 0;
		}
		/* period0 signal */
		else if(id == 0x11){
			changeEndianess(&rxFrame.dataWord0, &rxFrame.dataWord1);
			PRINTF("\r\nReceived message periods_setting (0x11)\r\n");
			PRINTF("period0 = 0x%x\r\n", rxFrame.dataByte3);
			PRINTF("New period = 0x%ds\r\n", rxFrame.dataByte3);
			id = 0;
		}
		else{
			delay(5);
		}
	}
    /* Stop FlexCAN Send & Receive. */
    FLEXCAN_DisableMbInterrupts(EXAMPLE_CAN, flag << RX_MESSAGE_BUFFER_0x10);
    FLEXCAN_DisableMbInterrupts(EXAMPLE_CAN, flag << RX_MESSAGE_BUFFER_0x11);

    LOG_INFO("\r\n==FlexCAN loopback functional example -- Finish.==\r\n");
}

void changeEndianess(uint32_t *word0, uint32_t *word1){
	uint32_t tempWord;

	tempWord = (*word0 & 0xFF000000) >> 24;
	tempWord = (tempWord | (*word0 & 0x00FF0000) >> 8);
	tempWord = (tempWord | (*word0 & 0x0000FF00) << 8);
	tempWord = (tempWord | (*word0 & 0x000000FF) << 24);
	*word0 = tempWord;

	tempWord = (*word1 & 0xFF000000) >> 24;
	tempWord = (tempWord | (*word1 & 0x00FF0000) >> 8);
	tempWord = (tempWord | (*word1 & 0x0000FF00) << 8);
	tempWord = (tempWord | (*word1 & 0x000000FF) << 24);
	*word1 = tempWord;
}

void delay(uint32_t seconds){
	for(uint32_t i=11000000*seconds;i>0;i--){
		;
	}
}
