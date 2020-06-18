/*
 * MEF.h
 *
 *  Created on: 10 jun. 2020
 *      Author: mauri
 */

#ifndef PROGRAMAS_TPRTOS_INC_MEF_H_
#define PROGRAMAS_TPRTOS_INC_MEF_H_

#include "sapi.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "board.h"
//===================================Declaracion de estados=======================//

#define PRESBTUP	0
#define PRESBTDOWM	1
#define PRESBTHOME	2
#define PRESBTEXIT	3

uint8_t activacion;

//=================================================================================//
typedef enum{PHOME,SET_P1,SET_P2,SET_P3,} MEFstate_t;

typedef struct {

	MEFstate_t state;
	uint8_t PRESBT;

}MEF_t;

struct Lectura_t Lectura;
MEF_t MEFF;

void (*pFunct)(void);
void  controlMEFInit(MEF_t * pMEF);
void controlconfig(MEF_t * pMEF);


#endif /* PROGRAMAS_TPRTOS_INC_MEF_H_ */
