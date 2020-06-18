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
#include "lcdtp.h"
#include "sensor.h"

static void actualizarsalidas(MEF_t * pMEF);


void controlMEFInit(MEF_t * pMEF){
	pMEF->state=PHOME;
	gpioWrite(LEDR, ON);
	pFunct = LCDhome;
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

					break;


		}

	}
	actualizarsalidas(pMEF);
}

static void actualizarsalidas(MEF_t * pMEF)
{
	switch(pMEF->state)
	{
	case PHOME:

		pFunct = LCDhome;
		activacion=FALSE;
		gpioWrite(LEDR,ON);
		gpioWrite(LEDG,OFF);
		gpioWrite(LEDB,OFF);
		gpioWrite(LED1,OFF);




		break;
	case SET_P1:

		pFunct = LCDP1;
		activacion=FALSE;
		gpioWrite(LEDG,ON);
		gpioWrite(LEDR,OFF);
		gpioWrite(LEDB,OFF);
		gpioWrite(LED1,OFF);




		break;

	case SET_P2:

		pFunct = LCDP2;
		activacion=TRUE;
		gpioWrite(LEDB,ON);
		gpioWrite(LEDR,OFF);
		gpioWrite(LEDG,OFF);
		gpioWrite(LED1,OFF);



		break;

	case SET_P3:

		pFunct = LCDP3;
		activacion=FALSE;
		gpioWrite(LED1,ON);
		gpioWrite(LEDR,OFF);
		gpioWrite(LEDG,OFF);
		gpioWrite(LEDB,OFF);


		break;


	}
}


