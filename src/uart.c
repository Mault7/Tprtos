/*
 * uart.c
 *
 *  Created on: 18 jun. 2020
 *      Author: mauri
 */
#include "FreeRTOS.h"
#include "semphr.h"
#include "sapi.h"
#include "queue.h"
#include "task.h"
#include <string.h>
#include "Tprtos.h"
#include <stdio.h>
#include <stdlib.h>


#define LENGTH 20
uint8_t buffuart[LENGTH];
uint8_t length;
bool_t new_cmd = false;

SemaphoreHandle_t semauart;

static void interruptuart(void *puart);
static void parser(void);

void uart_init(void)
{
	semauart=xSemaphoreCreateBinary();
	/* Inicializar la UART_USB junto con las interrupciones de Tx y Rx */
	uartConfig(UART_USB, 115200);
	// Seteo un callback al evento de recepcion y habilito su interrupcion
	uartCallbackSet(UART_USB, UART_RECEIVE, interruptuart, NULL);
	// Habilito todas las interrupciones de UART_USB
	uartInterrupt(UART_USB, false);
}


static void interruptuart(void *puart)
{

	BaseType_t xHigherPriorityTaskWoken;
	char c = uartRxRead( UART_USB );
	if(c=='A' && (!new_cmd))
	{
		length=0;
		new_cmd = true;
		memset(buffuart,0,LENGTH);
		buffuart[length++]=c;
	}
	else if(c=='\r')
	{

		xSemaphoreGiveFromISR( semauart, &xHigherPriorityTaskWoken );
		new_cmd = false;

	}
	else if(c!='\n')
	{
		buffuart[length++]=c;
	}
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void sensorconfig(void *param)
{
	for (;;) {
		xSemaphoreTake(semauart, portMAX_DELAY);
		parser();
	}

}

void parser(void)
{
	if(length==2)
	{
		uartWriteString(UART_USB,"OK\r\n");
	}
	else
	{
		uint8_t * cmd = buffuart + 3;
		if(!memcmp(cmd, "ADC", 3))
		{
			cmd += 3;
			if (!memcmp(cmd, "?", 1))
			{
				int range = get_ADC_range();
				uint8_t send_buff[15];
				sprintf(send_buff, "ADC=%d\r\n", range);
				uartWriteString(UART_USB, send_buff);
			}
			else if (!memcmp(cmd, "=", 1))
			{
				cmd++;
				int range = atoi(cmd);
				set_ADC_range(range);
				uint8_t send_buff[20];
				sprintf(send_buff, "OK\r\nADC=%d\r\n", range);
				uartWriteString(UART_USB, send_buff);
			}
		}
		else if (!memcmp(cmd, "INT", 3))
		{
			cmd += 3;
			if (!memcmp(cmd, "?", 1))
			{
				int intensity = get_INT_range();
				uint8_t send_buff[15];
				sprintf(send_buff, "INT=%d\r\n", intensity);
				uartWriteString(UART_USB, send_buff);
			}
			else if (!memcmp(cmd, "=", 1))
			{
				cmd++;
				int intensity = atoi(cmd);
				set_INT_range(intensity);
				uint8_t send_buff[20];
				sprintf(send_buff, "OK\r\nINT=%d\r\n", intensity);
				uartWriteString(UART_USB, send_buff);
			}
		}
	}
}

