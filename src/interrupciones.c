/*
 * interrupciones.c
 *
 *  Created on: 10 jun. 2020
 *      Author: mauri
 */

#include "sapi.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "interrupciones.h"
#include "semphr.h"
#include "board.h"


void My_IRQ_Init (void){
	//Inicializamos las interrupciones (LPCopen)
	Chip_PININT_Init(LPC_GPIO_PIN_INT);

	//Inicializamos de cada evento de interrupcion (LPCopen)

	/*
		GLOBAL! extern pinInitGpioLpc4337_t gpioPinsInit[];
		Chip_SCU_GPIOIntPinSel( j,  gpioPinsInit[i].gpio.port, gpioPinsInit[i].gpio.pin );   // TECi
		Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH( j ) );                      // INTj (canal j -> hanlder GPIOj)       //Borra el pending de la IRQ
		Chip_PININT_SetPinModeEdge( LPC_GPIO_PIN_INT, PININTCH( j ) );                      // INTj //Selecciona activo por flanco
		Chip_PININT_EnableIntLow( LPC_GPIO_PIN_INT, PININTCH( j ) );                        // INTj //Selecciona activo por flanco descendente
		Chip_PININT_EnableIntHigh( LPC_GPIO_PIN_INT, PININTCH( j ) );                       // INTj //Selecciona activo por flanco ascendente
	 */

	// TEC1 FALL
	Chip_SCU_GPIOIntPinSel(0, 0, 4); 	//(Canal 0 a 7, Puerto GPIO, Pin GPIO)
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH0);//Se configura el canal para que se active por flanco
	Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH0);//Se configura para que el flanco sea el de bajada

	// TEC1 RISE
	Chip_SCU_GPIOIntPinSel(1, 0, 4);	//(Canal 0 a 7, Puerto GPIO, Pin GPIO)
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH1);//Se configura el canal para que se active por flanco
	Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH1);//En este caso el flanco es de subida

	// TEC2 FALL
	Chip_SCU_GPIOIntPinSel(2, 0, 8);
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH2);
	Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH2);

	// TEC1 RISE
	Chip_SCU_GPIOIntPinSel(3, 0, 8);
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH3);
	Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH3);

	// TEC3 FALL
	Chip_SCU_GPIOIntPinSel(4, 0, 9);
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH4);
	Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH4);

	// TEC1 RISE
	Chip_SCU_GPIOIntPinSel(5, 0, 9);
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH5);
	Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH5);

	// TEC4 FALL
	Chip_SCU_GPIOIntPinSel(6, 1, 9);
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH6);
	Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH6);

	// TEC1 RISE
	Chip_SCU_GPIOIntPinSel(7, 1, 9);
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH7);
	Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH7);


	//Una vez que se han configurado los eventos para cada canal de interrupcion
	//Se activan las interrupciones para que comiencen a llamar al handler
	NVIC_SetPriority(PIN_INT0_IRQn, 2);
	NVIC_EnableIRQ(PIN_INT0_IRQn);
	NVIC_SetPriority(PIN_INT1_IRQn, 2);
	NVIC_EnableIRQ(PIN_INT1_IRQn);
	NVIC_SetPriority(PIN_INT2_IRQn, 2);
	NVIC_EnableIRQ(PIN_INT2_IRQn);
	NVIC_SetPriority(PIN_INT3_IRQn, 2);
	NVIC_EnableIRQ(PIN_INT3_IRQn);
	NVIC_SetPriority(PIN_INT4_IRQn, 2);
	NVIC_EnableIRQ(PIN_INT4_IRQn);
	NVIC_SetPriority(PIN_INT5_IRQn, 2);
	NVIC_EnableIRQ(PIN_INT5_IRQn);
	NVIC_SetPriority(PIN_INT6_IRQn, 2);
	NVIC_EnableIRQ(PIN_INT6_IRQn);
	NVIC_SetPriority(PIN_INT7_IRQn, 2);
	NVIC_EnableIRQ(PIN_INT7_IRQn);

}

void Tecla( void* taskParmPtr )
{
	// ---------- DefiniciÃ³n de variables locales ------------------------------
	struct Buttons_SM_t* Config; //Me preparo para recibir la direcciÃ³n de la estructura y copiarla en una varibale local
	Config = (struct Buttons_SM_t*) taskParmPtr;
	Config->Estado = UP;
	struct Lectura_t Lectura;
	Lectura.Tecla = Config->Tecla;

	struct Button_Control Control;
	TickType_t Last_Snapshot = 0;
/*
	if (pdTRUE == xSemaphoreTake( Mutex_uart, portMAX_DELAY)){

		printf("Tarea TEC%d iniciada\r\n",Lectura.Tecla+1);

		xSemaphoreGive( Mutex_uart );

	}

*/

	// ---------- REPETIR POR SIEMPRE --------------------------
	while(TRUE) {

		if (xQueueReceive(Config->Cola, &Control, portMAX_DELAY)){

			switch (Config->Estado){

			case UP:
				if(Control.Flanco == FALLING){ //AcÃ¡ adentro estÃ¡ el pseudo estado Falling

					if (pdFALSE == (xQueueReceive(Config->Cola, &Control, (ANTIREBOTE_MS / portTICK_RATE_MS)))){
						Config->Estado = DOWN;

						//AcÃ¡ se mete cÃ³digo para ejecutar en flanco  de bajada
						Config->Tiempo_inicial = Control.Tiempo_inicial;
/*
						if (pdTRUE == xSemaphoreTake( Mutex_uart, portMAX_DELAY)){

							printf("Se capturo una pulsacion\r\n");

							xSemaphoreGive( Mutex_uart );

						}

*/


					}
				}
				NVIC_EnableIRQ(PIN_INT0_IRQn);
				NVIC_EnableIRQ(PIN_INT2_IRQn);
				NVIC_EnableIRQ(PIN_INT4_IRQn);
				NVIC_EnableIRQ(PIN_INT6_IRQn);
				break;

			case DOWN:
				if(Control.Flanco == RISING){ //AcÃ¡ adentro estÃ¡ el pseudo estado Rising

					if (pdFALSE == (xQueueReceive(Config->Cola, &Control, (ANTIREBOTE_MS / portTICK_RATE_MS)))){
						Config->Estado = UP;

						//AcÃ¡ se mete cÃ³digo para ejecutar en flanco  de subida
//						Lectura.Tiempo_medido = xTaskGetTickCount() - Config->Tiempo_inicial;
/*
						if (pdTRUE == xSemaphoreTake( Mutex_uart, portMAX_DELAY)){

							printf("Lectura completada en la tecla %d, presionada por %dms\r\n",Lectura.Tecla+1,Lectura.Tiempo_medido*portTICK_RATE_MS);

							xSemaphoreGive( Mutex_uart );

						}

*/
						xQueueSend(Cola_Lecturas, &Lectura, portMAX_DELAY);
					}
				}
				NVIC_EnableIRQ(PIN_INT1_IRQn);
				NVIC_EnableIRQ(PIN_INT3_IRQn);
				NVIC_EnableIRQ(PIN_INT5_IRQn);
				NVIC_EnableIRQ(PIN_INT7_IRQn);
				break;

			default:
				Config->Estado = UP;
				break;

			}

		}


	}

}



void GPIO0_IRQHandler(void){
	NVIC_DisableIRQ(PIN_INT0_IRQn);
	BaseType_t xHigherPriorityTaskWoken = pdFALSE; //Comenzamos definiendo la variable


	if (Chip_PININT_GetFallStates(LPC_GPIO_PIN_INT) & PININTCH0){ //Verificamos que la interrupciÃ³n es la esperada
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH0); //Borramos el flag de interrupciÃ³n
		//codigo a ejecutar si ocurriÃ³ la interrupciÃ³n

		struct Button_Control Snapshot;
		Snapshot.Flanco = FALLING;
//		Snapshot.Tiempo_inicial = xTaskGetTickCountFromISR();

		xQueueSendFromISR( Buttons_SM[Tecla1].Cola, &Snapshot, &xHigherPriorityTaskWoken );


	}

	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO1_IRQHandler(void){
	NVIC_DisableIRQ(PIN_INT1_IRQn);
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;



	if (Chip_PININT_GetRiseStates(LPC_GPIO_PIN_INT) & PININTCH1){
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH1);
		//codigo a ejecutar si ocurriÃ³ la interrupciÃ³n
		struct Button_Control Snapshot;
		Snapshot.Flanco = RISING;
//		Snapshot.Tiempo_inicial = xTaskGetTickCountFromISR();
		xQueueSendFromISR( Buttons_SM[Tecla1].Cola, &Snapshot, &xHigherPriorityTaskWoken );
	}
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO2_IRQHandler(void){
	NVIC_DisableIRQ(PIN_INT2_IRQn);
	BaseType_t xHigherPriorityTaskWoken = pdFALSE; //Comenzamos definiendo la variable


	if (Chip_PININT_GetFallStates(LPC_GPIO_PIN_INT) & PININTCH2){ //Verificamos que la interrupciÃ³n es la esperada
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH2); //Borramos el flag de interrupciÃ³n
		//codigo a ejecutar si ocurriÃ³ la interrupciÃ³n
		struct Button_Control Snapshot;
		Snapshot.Flanco = FALLING;
//		Snapshot.Tiempo_inicial = xTaskGetTickCountFromISR();
		xQueueSendFromISR( Buttons_SM[Tecla2].Cola, &Snapshot, &xHigherPriorityTaskWoken );
	}

	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO3_IRQHandler(void){
	NVIC_DisableIRQ(PIN_INT3_IRQn);
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if (Chip_PININT_GetRiseStates(LPC_GPIO_PIN_INT) & PININTCH3){
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH3);
		//codigo a ejecutar si ocurriÃ³ la interrupciÃ³n
		struct Button_Control Snapshot;
		Snapshot.Flanco = RISING;
//		Snapshot.Tiempo_inicial = xTaskGetTickCountFromISR();
		xQueueSendFromISR( Buttons_SM[Tecla2].Cola, &Snapshot, &xHigherPriorityTaskWoken );
	}
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO4_IRQHandler(void){
	NVIC_DisableIRQ(PIN_INT4_IRQn);
	BaseType_t xHigherPriorityTaskWoken = pdFALSE; //Comenzamos definiendo la variable


	if (Chip_PININT_GetFallStates(LPC_GPIO_PIN_INT) & PININTCH4){ //Verificamos que la interrupciÃ³n es la esperada
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH4); //Borramos el flag de interrupciÃ³n
		//codigo a ejecutar si ocurriÃ³ la interrupciÃ³n
		struct Button_Control Snapshot;
		Snapshot.Flanco = FALLING;
//		Snapshot.Tiempo_inicial = xTaskGetTickCountFromISR();
		xQueueSendFromISR( Buttons_SM[Tecla3].Cola, &Snapshot, &xHigherPriorityTaskWoken );
	}

	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO5_IRQHandler(void){
	NVIC_DisableIRQ(PIN_INT5_IRQn);
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if (Chip_PININT_GetRiseStates(LPC_GPIO_PIN_INT) & PININTCH5){
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH5);
		//codigo a ejecutar si ocurriÃ³ la interrupciÃ³n
		struct Button_Control Snapshot;
		Snapshot.Flanco = RISING;
//		Snapshot.Tiempo_inicial = xTaskGetTickCountFromISR();
		xQueueSendFromISR( Buttons_SM[Tecla3].Cola, &Snapshot, &xHigherPriorityTaskWoken );
	}
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO6_IRQHandler(void){
	NVIC_DisableIRQ(PIN_INT6_IRQn);
	BaseType_t xHigherPriorityTaskWoken = pdFALSE; //Comenzamos definiendo la variable


	if (Chip_PININT_GetFallStates(LPC_GPIO_PIN_INT) & PININTCH6){ //Verificamos que la interrupciÃ³n es la esperada
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH6); //Borramos el flag de interrupciÃ³n
		//codigo a ejecutar si ocurriÃ³ la interrupciÃ³n
		struct Button_Control Snapshot;
		Snapshot.Flanco = FALLING;
//		Snapshot.Tiempo_inicial = xTaskGetTickCountFromISR();
		xQueueSendFromISR( Buttons_SM[Tecla4].Cola, &Snapshot, &xHigherPriorityTaskWoken );
	}

	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO7_IRQHandler(void){
	NVIC_DisableIRQ(PIN_INT7_IRQn);
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if (Chip_PININT_GetRiseStates(LPC_GPIO_PIN_INT) & PININTCH7){
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH7);
		//codigo a ejecutar si ocurriÃ³ la interrupciÃ³n
		struct Button_Control Snapshot;
		Snapshot.Flanco = RISING;
//		Snapshot.Tiempo_inicial = xTaskGetTickCountFromISR();
		xQueueSendFromISR( Buttons_SM[Tecla4].Cola, &Snapshot, &xHigherPriorityTaskWoken );
	}
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
