/*
 * r9sbus.h
 *
 *  Created on: 20 Ağu 2023
 *      Author: Administrator
 */

#ifndef INC_R9SBUS_H_
#define INC_R9SBUS_H_

#include "main_app.h"


#define R9SBUS_CHANNEL				USART1
#define R9SBUS_BAUDRATE				100000
#define R9SBUS_GPIO					GPIOA
#define R9SBUS_RXPIN				GPIO_PIN_10
#define R9SBUS_GPIO_AF        		GPIO_AF7_USART1
#define __R9SBUS_CLK_ENABLE()   	do { \
											__HAL_RCC_USART1_CLK_ENABLE(); \
											__HAL_RCC_GPIOA_CLK_ENABLE(); \
										} while(0U)
#define __R9SBUS_NVIC_ENABLE()   	do { \
											HAL_NVIC_SetPriority(USART1_IRQn, 10, 0); \
											HAL_NVIC_EnableIRQ(USART1_IRQn); \
										} while(0U)
#define R9SBUS_DATASIZE				25
#define R9SBUS_BUFFERSIZE			50
#define R9SBUS_CHANNELSIZE			16


#define R9SBUS_RECV_MIN           172.0f
#define R9SBUS_RECV_MAX           1811.0f
#define R9SBUS_OUT_MIN            -100.0f
#define R9SBUS_OUT_MAX            100.0f



typedef struct
{
	uint8_t							frame_lost;
	uint8_t                         buffer[R9SBUS_BUFFERSIZE];
	uint16_t						channels_raw[R9SBUS_CHANNELSIZE];
	float							channels[R9SBUS_CHANNELSIZE];
	uint8_t	 						ch17, ch18;
	uint8_t							failsafe;
	uint8_t							startbyte;
	uint8_t							endbyte;
	uint8_t							status;
}sR9SBUS;


extern sR9SBUS R9SBUS_data;

void R9SBUS_Init(void);
void R9SBUS_GPIOInit(UART_HandleTypeDef *huart);
void R9SBUS_RX_START_IT(void);
void R9SBUS_Parse_RXData(void);
void R9SBUS_Map_Channels(void);
void R9SBUS_RX_STARTtoIDLE_IT(void);
void R9SBUS_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
void R9SBUS_ErrorCallback(UART_HandleTypeDef *huart);

UART_HandleTypeDef hR9SBUS;


#endif /* INC_R9SBUS_H_ */
