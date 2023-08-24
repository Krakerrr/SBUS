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
											HAL_NVIC_SetPriority(USART1_IRQn, 15, 0); \
											HAL_NVIC_EnableIRQ(USART1_IRQn); \
										} while(0U)
#define R9SBUS_DATASIZE				25
#define R9SBUS_BUFFERSIZE			50



typedef struct
{
	uint8_t							frame_lost;
	uint8_t                         buffer[R9SBUS_BUFFERSIZE];
	uint16_t						channels[16];
	uint8_t	 						ch17, ch18;
	uint8_t							failsafe;
	uint8_t							endbyte;
	uint8_t							dataupdated;
}sR9SBUS_data;


extern sR9SBUS_data R9SBUS_data;

void R9SBUS_Init(void);
void R9SBUS_GPIOInit(void);
void R9SBUS_RX_START_IT(void);
void R9SBUS_Parse_RXData(void);
void R9SBUS_RX_STARTtoIDLE_IT(void);
void R9SBUS_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);

UART_HandleTypeDef hR9SBUS;


#endif /* INC_R9SBUS_H_ */
