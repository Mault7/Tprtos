/*
 * lcdtp.h
 *
 *  Created on: 13 jun. 2020
 *      Author: mauri
 */

#ifndef PROGRAMAS_TPPROTOCOLOS_INC_LCDTP_H_
#define PROGRAMAS_TPPROTOCOLOS_INC_LCDTP_H_

#include "sapi.h"

uint16_t contp1;
uint16_t contp2;
uint16_t contp3;
uint16_t contp4;

void LCDinit(void );
void LCDhome(void);
void LCDP1(void);
void LCDP2(void);
void LCDP3(void);


#endif /* PROGRAMAS_TPPROTOCOLOS_INC_LCDTP_H_ */
