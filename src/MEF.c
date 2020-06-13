/*
 * MEF.c
 *
 *  Created on: 10 jun. 2020
 *      Author: mauri
 */

#include "sapi.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "board.h"
#include "interrupciones.h"
#include "MEF.h"


static void actualizarsalidas(MEF_t * pMEF);


void controlMEFInit(MEF_t * pMEF){
	pMEF->state=PHOME;
	gpioWrite(LEDR, ON);
	//pFunct = funcionHome;
}


void controlconfig(MEF_t * pMEF){

	if(xQueueReceive(Cola_Lecturas, &Lectura, portMAX_DELAY)==pdTRUE)

	{

		switch(pMEF->state)
		{
		case PHOME:



			if (PRESBTHOME==Lectura.Tecla){
				pMEF->state=SET_P1;


			}

			break;
		case SET_P1:


			switch(Lectura.Tecla){
			case PRESBTHOME:
				pMEF->state=PHOME;
				break;
			case PRESBTUP:
				pMEF->state=SET_P2;
				break;
			case PRESBTDOWM:
				pMEF->state=SET_P3;
				break;
			default:
				break;
			}


//			if (PRESBTHOME==Lectura.Tecla){
//				pMEF->state=PHOME;
//			}
//			if(PRESBTUP==Lectura.Tecla){
//				pMEF->state=SET_P2;
//
//			}
//
//			if(PRESBTDOWM==Lectura.Tecla){
//				pMEF->state=SET_P3;
//			}

			break;

		case SET_P2:


			switch (Lectura.Tecla) {
			case PRESBTHOME:
				pMEF->state = PHOME;
				break;
			case PRESBTUP:
				pMEF->state = SET_P3;
				break;
			case PRESBTDOWM:
				pMEF->state = SET_P1;
				break;
			default:
				break;
			}

			break;

//			if(PRESBTHOME==Lectura.Tecla){
//				pMEF->state=PHOME;
//			}
//			if(PRESBTUP==Lectura.Tecla){
//				pMEF->state=SET_P3;
//			}
//
//			if(PRESBTDOWM==Lectura.Tecla){
//				pMEF->state=SET_P1;
//			}
//
//			break;

		case SET_P3:

			switch (Lectura.Tecla) {
			case PRESBTHOME:
				pMEF->state = PHOME;
				break;
			case PRESBTUP:
				pMEF->state = SET_P1;
				break;
			case PRESBTDOWM:
				pMEF->state = SET_P2;
				break;
			default:
				break;
			}

//			if(PRESBTHOME==Lectura.Tecla){
//				pMEF->state=PHOME;
//			}
//
//			if(PRESBTUP==Lectura.Tecla){
//				pMEF->state=SET_P1;
//			}
//
//			if(PRESBTDOWM==Lectura.Tecla){
//				pMEF->state=SET_P2;
//			}
			break;


		}

		//return TRUE;
	}
	actualizarsalidas(pMEF);
}

static void actualizarsalidas(MEF_t * pMEF)
{
	switch(pMEF->state)
	{
	case PHOME:


		gpioWrite(LEDR,ON);
		gpioWrite(LEDG,OFF);
		gpioWrite(LEDB,OFF);
		gpioWrite(LED1,OFF);




		break;
	case SET_P1:


		gpioWrite(LEDG,ON);
		gpioWrite(LEDR,OFF);
		gpioWrite(LEDB,OFF);
		gpioWrite(LED1,OFF);




		break;

	case SET_P2:


		gpioWrite(LEDB,ON);
		gpioWrite(LEDR,OFF);
		gpioWrite(LEDG,OFF);
		gpioWrite(LED1,OFF);



		break;

	case SET_P3:

		gpioWrite(LED1,ON);
		gpioWrite(LEDR,OFF);
		gpioWrite(LEDG,OFF);
		gpioWrite(LEDB,OFF);


		break;


	}
}


