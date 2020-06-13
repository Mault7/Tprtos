/*=============================================================================
 * Copyright (c) 2020, Mauricio Lara <torneosmau@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2020/06/10
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/




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

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
	boardConfig();

	// Create a task in freeRTOS with dynamic memory
	My_IRQ_Init();

	//delayInaccurateMs(1000);

	LCDinit();
//	LCDhome();


	controlMEFInit(&MEFF);





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
			configMINIMAL_STACK_SIZE*3, // Cantidad de stack de la tarea
			&Buttons_SM[0],                 // Parametros de tarea
			tskIDLE_PRIORITY+2,         // Prioridad de la tarea
			0                           // Puntero a la tarea creada en el sistema
	);


	xTaskCreate(
			Tecla,                     // Funcion de la tarea a ejecutar
			(const char *)"Tec2",     // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE*3, // Cantidad de stack de la tarea
			&Buttons_SM[1],                          // Parametros de tarea
			tskIDLE_PRIORITY+2,         // Prioridad de la tarea
			0                           // Puntero a la tarea creada en el sistema
	);


	xTaskCreate(
			Tecla,                     // Funcion de la tarea a ejecutar
			(const char *)"Tec3",     // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE*3, // Cantidad de stack de la tarea
			&Buttons_SM[2],                         // Parametros de tarea
			tskIDLE_PRIORITY+2,         // Prioridad de la tarea
			0                           // Puntero a la tarea creada en el sistema
	);


	xTaskCreate(
			MEF,                     // Funcion de la tarea a ejecutar
			(const char *)"MEF",     // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE*1, // Cantidad de stack de la tarea
			0,                          // Parametros de tarea
			tskIDLE_PRIORITY+3,         // Prioridad de la tarea
			0                           // Puntero a la tarea creada en el sistema
	);

	xTaskCreate(
			Eventos,
			(const char *)"Eventos",
			configMINIMAL_STACK_SIZE,
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

	portTickType xPeriodicity = 300 / portTICK_RATE_MS;
	portTickType xLastWakeTime = xTaskGetTickCount();

	while (TRUE)
	{

		pFunct();

		vTaskDelayUntil(&xLastWakeTime, xPeriodicity);

	}

}





