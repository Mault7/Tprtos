/*
 * interrupciones.h
 *
 *  Created on: 10 jun. 2020
 *      Author: mauri
 */

#ifndef PROGRAMAS_TPRTOS_INC_INTERRUPCIONES_H_
#define PROGRAMAS_TPRTOS_INC_INTERRUPCIONES_H_

#include "sapi.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "board.h"

//=======================definicion de estados===============//

#define UP      1
#define FALLING 2
#define DOWN	3
#define RISING  4

//======================definicion cantidad de teclas===========//

#define CANT_TECLAS 3
#define CANT_LEDS 4
#define ANTIREBOTE_MS 40

uint8_t Error_state;

enum Teclas_t {Tecla1, Tecla2, Tecla3, Tecla4}; //indices de teclas para el vector de estructuras



struct Button_Control { //estructura de control de datos capturados por la interrupciÃ³n
	TickType_t Tiempo_inicial;
	uint8_t Flanco;
};

struct Buttons_SM_t{ //estructura de control de la mÃ¡quina de estados de cada botÃ³n
	uint8_t Tecla;
	uint8_t Estado;
	xQueueHandle Cola;
	TickType_t Tiempo_inicial;
};

struct Lectura_t{
	uint8_t Tecla;
	TickType_t Tiempo_medido;
};

//===============================definicion de datos internos====================================
 //Mutex que protege la UART de concurrencia

xQueueHandle Cola_Lecturas;
xQueueHandle Cola_MEF;

//DefiniciÃ³n de vector de estructuras de control
struct Buttons_SM_t Buttons_SM[CANT_TECLAS];


//======================funciones para inicializacion y obtencion de tecla=======================


void My_IRQ_Init (void);
void Tecla( void* taskParmPtr );





#endif /* PROGRAMAS_TPRTOS_INC_INTERRUPCIONES_H_ */
