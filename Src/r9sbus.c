



#include "r9sbus.h"

const float R9SBUS_gain   = (R9SBUS_OUT_MAX - R9SBUS_OUT_MIN) / (R9SBUS_RECV_MAX - R9SBUS_RECV_MIN) ;
const float R9SBUS_offset = R9SBUS_OUT_MIN - R9SBUS_gain * R9SBUS_RECV_MIN;


void R9SBUS_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == R9SBUS_CHANNEL)
	{
		if(huart->ErrorCode & HAL_UART_ERROR_PE)
		{
			/* Parity error */
			UARTprintmsg("Parity error \n");
		}

		if(huart->ErrorCode & HAL_UART_ERROR_NE)
		{
			/* noise error */
			UARTprintmsg("noise error \n");
		}

		if(huart->ErrorCode & HAL_UART_ERROR_FE)
		{
			/* frame error */
			UARTprintmsg("frame error \n");
		}

		if(huart->ErrorCode & HAL_UART_ERROR_ORE)
		{
			/* overrun error */
			UARTprintmsg("overrun error \n");
		}

		R9SBUS_RX_STARTtoIDLE_IT();
	}
}

void R9SBUS_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == R9SBUS_CHANNEL) {
		if( Size == 25)
		{
			R9SBUS_Parse_RXData();
		}
		R9SBUS_RX_STARTtoIDLE_IT();
	}

}
void R9SBUS_RX_STARTtoIDLE_IT(void)
{
	R9SBUS_data.status = 0;
	if( HAL_UARTEx_ReceiveToIdle_IT(&hR9SBUS, R9SBUS_data.buffer, R9SBUS_DATASIZE) != HAL_OK)
	{
		UARTprintmsg("Error in R9SBUS_RX_STARTtoIDLE_IT\r\n");
	}

}

void R9SBUS_RX_START_IT(void)
{
	R9SBUS_data.status = 0;
	if( HAL_UART_Receive_IT(&hR9SBUS, R9SBUS_data.buffer, R9SBUS_DATASIZE) != HAL_OK)
	{
		UARTprintmsg("Error in R9SBUS_RX_START_IT\r\n");
	}
}

void R9SBUS_Map_Channels(void)
{
	for (uint8_t i = 0;  i < R9SBUS_CHANNELSIZE; ++ i)
	{
		R9SBUS_data.channels[i] = R9SBUS_gain * (float)R9SBUS_data.channels_raw[i] + R9SBUS_offset;
	}
}


void R9SBUS_Validate_RXData(void)
{
	if( (R9SBUS_data.startbyte == 0x0f) && !R9SBUS_data.endbyte && !R9SBUS_data.frame_lost && !R9SBUS_data.failsafe)
	{
		R9SBUS_data.status = 0;
	}else
	{
		R9SBUS_data.status = 1;
	}
}

void R9SBUS_Parse_RXData(void)
{
	// Decode the 16 regular channels
	R9SBUS_data.channels_raw[0]  = (((uint16_t)R9SBUS_data.buffer[1])       | ((uint16_t)R9SBUS_data.buffer[2] << 8) ) & 0x07FF;
	R9SBUS_data.channels_raw[1]  = (((uint16_t)R9SBUS_data.buffer[2] >> 3)  | ((uint16_t)R9SBUS_data.buffer[3] << 5)) & 0x07FF;
	R9SBUS_data.channels_raw[2]  = (((uint16_t)R9SBUS_data.buffer[3] >> 6)  | ((uint16_t)R9SBUS_data.buffer[4] << 2) | ((uint16_t)R9SBUS_data.buffer[5] << 10)) & 0x07FF;
	R9SBUS_data.channels_raw[3]  = (((uint16_t)R9SBUS_data.buffer[5] >> 1)  | ((uint16_t)R9SBUS_data.buffer[6] << 7)) & 0x07FF;
	R9SBUS_data.channels_raw[4]  = (((uint16_t)R9SBUS_data.buffer[6] >> 4)  | ((uint16_t)R9SBUS_data.buffer[7] << 4)) & 0x07FF;
	R9SBUS_data.channels_raw[5]  = (((uint16_t)R9SBUS_data.buffer[7] >> 7)  | ((uint16_t)R9SBUS_data.buffer[8] << 1) | ((uint16_t)R9SBUS_data.buffer[9] << 9)) & 0x07FF;
	R9SBUS_data.channels_raw[6]  = (((uint16_t)R9SBUS_data.buffer[9] >> 2)  | ((uint16_t)R9SBUS_data.buffer[10] << 6)) & 0x07FF;
	R9SBUS_data.channels_raw[7]  = (((uint16_t)R9SBUS_data.buffer[10] >> 5) | ((uint16_t)R9SBUS_data.buffer[11] << 3)) & 0x07FF;
	R9SBUS_data.channels_raw[8]  = (((uint16_t)R9SBUS_data.buffer[12])      | ((uint16_t)R9SBUS_data.buffer[13] << 8)) & 0x07FF;
	R9SBUS_data.channels_raw[9]  = (((uint16_t)R9SBUS_data.buffer[13] >> 3) | ((uint16_t)R9SBUS_data.buffer[14] << 5)) & 0x07FF;
	R9SBUS_data.channels_raw[10] = (((uint16_t)R9SBUS_data.buffer[14] >> 6) | ((uint16_t)R9SBUS_data.buffer[15] << 2) | ((uint16_t)R9SBUS_data.buffer[16] << 10)) & 0x07FF;
	R9SBUS_data.channels_raw[11] = (((uint16_t)R9SBUS_data.buffer[16] >> 1) | ((uint16_t)R9SBUS_data.buffer[17] << 7)) & 0x07FF;
	R9SBUS_data.channels_raw[12] = (((uint16_t)R9SBUS_data.buffer[17] >> 4) | ((uint16_t)R9SBUS_data.buffer[18] << 4)) & 0x07FF;
	R9SBUS_data.channels_raw[13] = (((uint16_t)R9SBUS_data.buffer[18] >> 7) | ((uint16_t)R9SBUS_data.buffer[19] << 1) | ((uint16_t)R9SBUS_data.buffer[20] << 9)) & 0x07FF;
	R9SBUS_data.channels_raw[14] = (((uint16_t)R9SBUS_data.buffer[20] >> 2) | ((uint16_t)R9SBUS_data.buffer[21] << 6)) & 0x07FF;
	R9SBUS_data.channels_raw[15] = (((uint16_t)R9SBUS_data.buffer[21] >> 5) | ((uint16_t)R9SBUS_data.buffer[22] << 3)) & 0x07FF;

	R9SBUS_Map_Channels();

	/* CH 17 */
	R9SBUS_data.ch17 = R9SBUS_data.buffer[23] & 0x01;
	/* CH 18 */
	R9SBUS_data.ch18 = R9SBUS_data.buffer[23] & 0x02;

	/* Grab the lost frame */
	R9SBUS_data.frame_lost = R9SBUS_data.buffer[23] & 0x04;
	/* Grab the failsafe */
	R9SBUS_data.failsafe = R9SBUS_data.buffer[23] & 0x08;
	/*Start byte */
	R9SBUS_data.startbyte = R9SBUS_data.buffer[0];
	/*End byte */
	R9SBUS_data.endbyte = R9SBUS_data.buffer[25];


	R9SBUS_Map_Channels();
	R9SBUS_Validate_RXData();
}

void R9SBUS_Init(void)
{
	hR9SBUS.Instance 			= R9SBUS_CHANNEL;
	hR9SBUS.Init.BaudRate		= R9SBUS_BAUDRATE;
	hR9SBUS.Init.WordLength 	= UART_WORDLENGTH_9B;
	hR9SBUS.Init.StopBits 		= UART_STOPBITS_2;
	hR9SBUS.Init.Parity 		= UART_PARITY_EVEN;
	hR9SBUS.Init.Mode 			= UART_MODE_RX;
	hR9SBUS.Init.HwFlowCtl 		= UART_HWCONTROL_NONE;
	hR9SBUS.Init.OverSampling 	= UART_OVERSAMPLING_8;
	if ( HAL_UART_Init(&hR9SBUS) != HAL_OK )
	{
		UARTprintmsg("Error in UARTR9SBUS_Init\r\n");
		Error_Handler();
	}
}

void R9SBUS_GPIOInit(void)
{
	GPIO_InitTypeDef gpio_uart;

	//	1. enable peripheral clock usart and gpioa
	__R9SBUS_CLK_ENABLE();

	//	2. pin muxing configurations (tx rx pins / 2gpios : alternate func. -> UART)
	gpio_uart.Pin   	= R9SBUS_RXPIN;
	gpio_uart.Mode  	= GPIO_MODE_AF_PP;
	gpio_uart.Pull  	= GPIO_PULLUP;
	gpio_uart.Speed 	= GPIO_SPEED_FREQ_VERY_HIGH;
	gpio_uart.Alternate = R9SBUS_GPIO_AF;
	HAL_GPIO_Init( R9SBUS_GPIO, &gpio_uart);

	//	3. set priority
	//	4. enable peripheral irq in the nvic
	__R9SBUS_NVIC_ENABLE();
}
