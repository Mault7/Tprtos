/*
 * hearRate.h
 *
 *  Created on: 14 jun. 2020
 *      Author: mauri
 */

#ifndef PROGRAMAS_TPRTOS_INC_SENSOR_H_
#define PROGRAMAS_TPRTOS_INC_SENSOR_H_

#include "sapi.h"

typedef struct
{
	uint8_t sensortemp;
	int32_t senstofrec;
}sensor_t;


typedef enum {FRECZERO,FRECNONZERO,}statefrec_t;


typedef struct
{
	uint32_t umbral;
	statefrec_t statefrec;
	uint8_t contfrec;
	bool_t fczero;
	bool_t fcnonzero;

}frec_t;

frec_t frec;


sensor_t datosen;







#endif /* PROGRAMAS_TPRTOS_INC_SENSOR_H_ */
