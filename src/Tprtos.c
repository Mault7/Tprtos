/*=============================================================================
 * Copyright (c) 2020, Mauricio Lara <torneosmau@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2020/06/10
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/




#include "sensor.h"
#include "userTasks.h"
#include "interrupciones.h"
#include "queue.h"
#include "task.h"
#include "FreeRTOS.h"   //Motor del OS
#include "semphr.h"		//Api de sincronizaciÃ³n (sem y mutex)
#include "sapi.h"
#include "FreeRTOSConfig.h"
#include "board.h"
#include "MEF.h"
#include "lcdtp.h"
#include "max30105.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/


//===================== datos sensor max30105================================//
uint8_t ledBrightness = 0x1F; //Options: 0=Off to 255=50mA
uint8_t sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
uint8_t ledMode = 3; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
int sampleRate = 400; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
int pulseWidth = 411; //Options: 69, 118, 215, 411
int adcRange = 4096; //Options: 2048, 4096, 8192, 16384
sense_struct sense;
//===========================================================================//
float y;
float alpha=0.05;
float s;
//=================== datos para frecuencia cardiaca=========================//
uint32_t timesample=0;
bool_t accont=FALSE;
//==========================================================================//


/*=====[Definitions of private global variables]=============================*/
void uart(uint32_t dato){
	char uartBuff[10];
	itoa(dato,uartBuff,10);
	uartWriteString( UART_USB, uartBuff );
	uartWriteString( UART_USB, "\r\n" );
}
/*=====[Main function, program entry point after power on or reset]==========*/
uint32_t countertick;
int main( void )
{
	boardConfig();

	// Create a task in freeRTOS with dynamic memory
	My_IRQ_Init();
	uartConfig(UART_USB,115200);
	//delayInaccurateMs(1000);

	LCDinit();
	//	LCDhome();
	i2cInit(I2C0,400000);

	controlMEFInit(&MEFF);

	setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
	setPulseAmplitudeRed(0x0A);
	setPulseAmplitudeGreen(0x0);



	//========================creacion de colas para capturar teclas===============================
	for (int i = CANT_TECLAS ; i-- ; i >= 0) {
		Buttons_SM[i].Tecla = i;
		if (NULL == (Buttons_SM[i].Cola = xQueueCreate(3,sizeof(struct Button_Control)))){
			Error_state =1;
		}
	}
	//===================================Creamos cola de lecturas completadas ========================//
	if (NULL == (Cola_Lecturas = xQueueCreate(1,sizeof(struct Lectura_t)))){
		Error_state =1;
	}


	xTaskCreate(
			Tecla,                     // Funcion de la tarea a ejecutar
			(const char *)"Tec1",     // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE*1, // Cantidad de stack de la tarea
			&Buttons_SM[0],                 // Parametros de tarea
			tskIDLE_PRIORITY+2,         // Prioridad de la tarea
			0                           // Puntero a la tarea creada en el sistema
	);


	xTaskCreate(
			Tecla,                     // Funcion de la tarea a ejecutar
			(const char *)"Tec2",     // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE*1, // Cantidad de stack de la tarea
			&Buttons_SM[1],                          // Parametros de tarea
			tskIDLE_PRIORITY+2,         // Prioridad de la tarea
			0                           // Puntero a la tarea creada en el sistema
	);


	xTaskCreate(
			Tecla,                     // Funcion de la tarea a ejecutar
			(const char *)"Tec3",     // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE*1, // Cantidad de stack de la tarea
			&Buttons_SM[2],                         // Parametros de tarea
			tskIDLE_PRIORITY+2,         // Prioridad de la tarea
			0                           // Puntero a la tarea creada en el sistema
	);


	xTaskCreate(
			MEF,                     // Funcion de la tarea a ejecutar
			(const char *)"MEF",     // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE*1, // Cantidad de stack de la tarea
			0,                          // Parametros de tarea
			tskIDLE_PRIORITY+2,         // Prioridad de la tarea
			0                           // Puntero a la tarea creada en el sistema
	);

	xTaskCreate(
			Eventos,
			(const char *)"Eventos",
			configMINIMAL_STACK_SIZE*1,
			0,
			tskIDLE_PRIORITY+2,
			0
	);

	xTaskCreate(
			Sensor,
			(const char *)"Sensor",
			configMINIMAL_STACK_SIZE*4,
			0,
			tskIDLE_PRIORITY+2,
			0
	);

	xTaskCreate(
			Sensortemp,
			(const char *)"Sensortemp",
			configMINIMAL_STACK_SIZE*3,
			0,
			tskIDLE_PRIORITY+1,
			0
	);

	// Iniciar scheduler
	if (0 == Error_state){
		printf("iniciando");
	} else{
		printf("Error al iniciar el sistema !!!!!!!!!!!!!!");
	}


	vTaskStartScheduler(); // Initialize scheduler

	while( true ); // If reach heare it means that the scheduler could not start

	// YOU NEVER REACH HERE, because this program runs directly or on a
	// microcontroller and is not called by any Operating System, as in the
	// case of a PC program.
	return 0;

}

void MEF( void* taskParmPtr ){



	while (TRUE){


		controlconfig(&MEFF);

	}
}



void Eventos( void* taskParmPtr ){

	portTickType xPeriodicity = 10 / portTICK_RATE_MS;
	portTickType xLastWakeTime = xTaskGetTickCount();

	while (TRUE)
	{

		pFunct();

		vTaskDelayUntil(&xLastWakeTime, xPeriodicity);

	}

}


void Sensor( void* taskParmPtr ){


	portTickType xPeriodicity = 1000 / portTICK_RATE_MS;
	portTickType xLastWakeTime = xTaskGetTickCount();




	while (TRUE){

		if(accont==TRUE)
		{
			timesample++;
			if(timesample>10)
			{
				frec.contfrec=(frec.contfrec*6)+27;
				datosen.senstofrec=frec.contfrec;
				timesample=0;
				frec.contfrec=0;
				accont=FALSE;
			}
		}


		//		check(&sense);
		//		int32_t irValue=getFIFOIR(&sense);
		uint8_t temp=readTemperature();

		datosen.sensortemp=temp;

		vTaskDelayUntil(&xLastWakeTime, xPeriodicity);


	}
}



void Sensortemp( void* taskParmPtr ){




	int32_t temp;
	frec.umbral=6000;
	frec.statefrec=FRECRISING;

	while (TRUE){


		uint32_t irvalue=getIR(&sense);
		y=(float)irvalue;
		s=(alpha*y)+((1-alpha)*s);
		int32_t diff=(int32_t)s;

		temp=(int32_t)diff-(int32_t)120000;

		//if(temp>0){
		if(temp>0){



			switch(frec.statefrec)
			{
			case FRECRISING:

				if(temp>frec.umbral)
				{
					frec.fcfalling=FALSE;
					frec.fcrising=TRUE;
				}
				else
				{
					frec.statefrec=FRECFALLING;
				}
				break;

			case FRECFALLING:

				if(temp>frec.umbral)
				{

					frec.contfrec++;
					frec.statefrec=FRECRISING;
					uart(frec.contfrec);
				}
				break;
			}
		}

			if(frec.contfrec!=0)
			{
				accont=TRUE;
			}
			else{
				accont=FALSE;
			}

			uart(timesample);

		vTaskDelay(16/portTICK_RATE_MS);
	}
}
