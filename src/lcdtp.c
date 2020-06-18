/*
 * lcdtp.c
 *
 *  Created on: 13 jun. 2020
 *      Author: mauri
 */

#include "lcdtp.h"
#include "sapi.h"
#include "sensor.h"

const char PART1[8] = {
		0b00111,
		0b01111,
		0b11100,
		0b11000,
		0b11000,
		0b11100,
		0b01111,
		0b00111
};

// Caracter personalizado letra r
const char PART2[8] = {
		0b11000,
		0b11000,
		0b11000,
		0b11000,
		0b11001,
		0b11001,
		0b11011,
		0b11011
};

// Caracter personalizado letra i
const char PART3[8] = {
		0b01110,
		0b11111,
		0b11011,
		0b11011,
		0b10001,
		0b10001,
		0b00000,
		0b00000
};

// Caracter personalizado letra c
const char PART4[8] = {
		0b11000,
		0b11000,
		0b01100,
		0b01100,
		0b10110,
		0b10110,
		0b11011,
		0b11011
};

//Temperatura - Termometro
const char PART5[8] = {
		0b11000,
		0b11100,
		0b01110,
		0b00111,
		0b01110,
		0b11100,
		0b11000,
		0b00000
};

// Humedad - Gota
const char PART6[8] = {
		0b00000,
		0b00000,
		0b11011,
		0b11111,
		0b01110,
		0b00100,
		0b00000,
		0b00100
};

// Viento
const char PART7[8] = {
		0b01110,
		0b01010,
		0b01010,
		0b01010,
		0b01110,
		0b10111,
		0b01110,
		0b00000
};



enum{
	PART1_CHAR =0,
	PART2_CHAR =1,
	PART3_CHAR =2,
	PART4_CHAR =3,
	PART5_CHAR =4,
	PART6_CHAR =5,
	PART7_CHAR =6,

};
void LCDinit(void ){

	contp1=0;
	contp2=0;
	contp3=0;
	contp4=0;

	datosen.sensortemp=0;
	datosen.senstofrec=0;

	lcdInit( 16, 2, 5, 8 );


	lcdCreateChar( PART1_CHAR, PART1 );
	lcdCreateChar( PART2_CHAR, PART2 );
	lcdCreateChar( PART3_CHAR, PART3 );
	lcdCreateChar( PART4_CHAR, PART4 );
	lcdCreateChar( PART5_CHAR, PART5 );
	lcdCreateChar( PART6_CHAR, PART6 );
	lcdCreateChar( PART7_CHAR, PART7 );

	lcdCursorSet( LCD_CURSOR_OFF );
	lcdClear();
}



void LCDhome(void){

	if(contp1==20){
		lcdGoToXY( 0, 0 );
		lcdSendStringRaw( "BIENVENIDOS  AL" );

		lcdGoToXY( 0, 1 );
		lcdSendStringRaw( "PROYECTO" );



		lcdGoToXY( 12, 1 );
		lcdData(PART1_CHAR);
		lcdData(PART2_CHAR);
		lcdData(PART3_CHAR);
		lcdData(PART4_CHAR);
	}

	if(contp1==40){
		lcdClear();

		lcdGoToXY( 0, 0 );
		lcdSendStringRaw( "BIENVENIDOS  AL" );

		lcdGoToXY( 0, 1 );
		lcdSendStringRaw( "PROYECTO" );

		lcdData(PART5_CHAR);

		lcdGoToXY( 12, 1 );
		lcdData(PART1_CHAR);
		lcdData(PART2_CHAR);
		lcdData(PART3_CHAR);
		lcdData(PART4_CHAR);
	}

	if(contp1==60){
		lcdClear();

		lcdGoToXY( 0, 0 );
		lcdSendStringRaw( "BIENVENIDOS  AL" );

		lcdGoToXY( 0, 1 );
		lcdSendStringRaw( "PROYECTO" );

		lcdData(PART5_CHAR);
		lcdData(PART5_CHAR);

		lcdGoToXY( 12, 1 );
		lcdData(PART1_CHAR);
		lcdData(PART2_CHAR);
		lcdData(PART3_CHAR);
		lcdData(PART4_CHAR);
	}

	if(contp1==80){
		lcdClear();

		lcdGoToXY( 0, 0 );
		lcdSendStringRaw( "BIENVENIDOS  AL" );

		lcdGoToXY( 0, 1 );
		lcdSendStringRaw( "PROYECTO" );

		lcdData(PART5_CHAR);
		lcdData(PART5_CHAR);
		lcdData(PART5_CHAR);

		lcdGoToXY( 12, 1 );
		lcdData(PART1_CHAR);
		lcdData(PART2_CHAR);
		lcdData(PART3_CHAR);
		lcdData(PART4_CHAR);
	}

	if(contp1==100){
		lcdGoToXY( 0, 0 );
		lcdSendStringRaw( "BIENVENIDOS  AL" );

		lcdGoToXY( 0, 1 );
		lcdSendStringRaw( "PROYECTO" );

		lcdData(PART5_CHAR);
		lcdData(PART5_CHAR);
		lcdData(PART5_CHAR);
		lcdData(PART5_CHAR);

		lcdGoToXY( 12, 1 );
		lcdData(PART1_CHAR);
		lcdData(PART2_CHAR);
		lcdData(PART3_CHAR);
		lcdData(PART4_CHAR);
	}

	if(contp1==120){
		lcdClear();

		lcdGoToXY( 0, 0 );
		lcdSendStringRaw( "BIENVENIDOS  AL" );

		lcdGoToXY( 0, 1 );
		lcdSendStringRaw( "PROYECTO" );


		lcdGoToXY( 12, 1 );
		lcdData(PART1_CHAR);
		lcdData(PART2_CHAR);
		lcdData(PART3_CHAR);
		lcdData(PART4_CHAR);
	}

	if(contp1>120)contp1=0;
	contp1++;
}


void LCDP1(void)
{

	if(contp2==20){
		lcdClear();
		lcdGoToXY( 0, 0 );
		lcdSendStringRaw( "*TEMP  CORPORAL*" );

		lcdGoToXY( 0, 1 );
		lcdData(PART1_CHAR);
		lcdData(PART2_CHAR);
		lcdData(PART3_CHAR);
		lcdData(PART4_CHAR);

		lcdGoToXY( 5, 1 );
		lcdSendStringRaw( "*C " );
		lcdSendInt(datosen.sensortemp);


		lcdGoToXY( 15, 1 );
		lcdData(PART7_CHAR);
	}

	if(contp2==100){
		lcdClear();
		lcdGoToXY( 0, 0 );
		lcdSendStringRaw( "*TEMP  CORPORAL*" );

		lcdGoToXY( 0, 1 );
		lcdData(PART1_CHAR);
		lcdData(PART2_CHAR);
		lcdData(PART3_CHAR);
		lcdData(PART4_CHAR);

		lcdGoToXY( 5, 1 );
		lcdSendStringRaw( "*C " );
		lcdSendInt(datosen.sensortemp);


	}

	if(contp2>100)contp2=0;
	contp2++;

}


void LCDP2(void)
{

	if(contp3==20){
		lcdClear();
		lcdGoToXY( 0, 0 );
		lcdSendStringRaw( "*FREC  CARDIACA*" );

		lcdGoToXY( 0, 1 );
		lcdData(PART1_CHAR);
		lcdData(PART2_CHAR);
		lcdData(PART3_CHAR);
		lcdData(PART4_CHAR);


		lcdGoToXY( 6, 1 );
		lcdSendStringRaw( "BPM " );
		lcdSendInt(datosen.senstofrec);


		lcdGoToXY( 15, 1 );
		lcdData(PART6_CHAR);
	}

	if(contp3==100){
		lcdClear();
		lcdGoToXY( 0, 0 );
		lcdSendStringRaw( "*FREC  CARDIACA*" );

		lcdGoToXY( 0, 1 );
		lcdData(PART1_CHAR);
		lcdData(PART2_CHAR);
		lcdData(PART3_CHAR);
		lcdData(PART4_CHAR);

		lcdGoToXY( 6, 1 );
		lcdSendStringRaw( "BPM " );
		lcdSendInt(datosen.senstofrec);

	}

	if(contp3>100)contp3=0;
	contp3++;
}


void LCDP3(void)
{

	if(contp4==20){
		lcdClear();
		lcdGoToXY( 0, 0 );
		lcdSendStringRaw( "*CONFIG  SENSOR*" );

		lcdGoToXY( 0, 1 );
		lcdData(PART1_CHAR);
		lcdData(PART2_CHAR);
		lcdData(PART3_CHAR);
		lcdData(PART4_CHAR);
	}
	if(contp4>20)contp4=0;
	contp4++;
}



