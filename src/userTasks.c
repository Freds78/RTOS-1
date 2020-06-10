/*=============================================================================
 * Author: Freds Amundaray Cruz <freds.amundaray@gmail.com>
 * Date: 2020/06/07
 * Version: 1
 *===========================================================================*/

/*=====[Inclusion of own header]=============================================*/

#include "userTasks.h"
#include "TempFreeRTOS.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "sapi.h"
#include "Temp.h"
#include "semphr.h"
#include "queue.h"
#include "supporting_functions.h"
#include "ff.h"       // <= Biblioteca FAT FS
#include "fssdc.h"	// API de bajo nivel para unidad "SDC:" en FAT FS

/*=====[Inclusions of private function dependencies]=========================*/

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/
#define FILENAME "SDC:/Temperature.txt"
static  FIL fp;

/*=====[Implementations of public functions]=================================*/

// Task implementation

void muestreoTask( void* taskParmPtr ){

	// ---------- CONFIGURACIONES ------------------------------
	sensor_t *pSensor  = (sensor_t*)taskParmPtr;
	QueueHandle_t muestra;
	SemaphoreHandle_t mutex;
	uint16_t ValorADC = 0;
	uint16_t Temperatura = 0;
	int32_t Valor;

	// ---------- REPETIR POR SIEMPRE --------------------------
	for( ;; )
	{
		ValorADC = adcRead( CH1 );
		Temperatura = (400*ValorADC)/1024;
		Valor = (int32_t)Temperatura;

		xSemaphoreTake( mutex , portMAX_DELAY );			//abrir seccion critica
		vPrintStringAndNumber( "Temperature is:", Valor );
		xSemaphoreGive( mutex );


		//xStatus = xQueueSendToBack ( pSensor->muestra , &Valor,  portMAX_DELAY  );
		if(xQueueSend(muestra , &Valor,  portMAX_DELAY )!= pdTRUE){
			vPrintString( "Could not send from the queue.\r\n" );
		}
	}
}

void Led1_LowTask( void *taskParmPtr )
{
	// ---------- CONFIGURACIONES ------------------------------
	sensor_t *pSensor  = (sensor_t*)taskParmPtr;
	QueueHandle_t muestra;
	int32_t Led1;
	TickType_t xPeriodicity =  1000 / portTICK_RATE_MS;		// Tarea periodica cada 1000 ms
	TickType_t xLastWakeTime = xTaskGetTickCount();

	// ---------- REPETIR POR SIEMPRE --------------------------
	for(;;){

		if (xQueueReceive( muestra, &Led1, portMAX_DELAY )== pdTRUE){
			if(Led1 <= 100){
				gpioWrite( LED1 , 1 );
				vTaskDelay( 500 / portTICK_RATE_MS);
				gpioWrite( LED1 , 0 );
			}
		}else{
			vPrintString( "Could not receive from the queue.\r\n" );
		}
		vTaskDelayUntil( &xLastWakeTime , xPeriodicity );
	}
}

void Led2_HightTask( void *taskParmPtr )
{
	// ---------- CONFIGURACIONES ------------------------------
	sensor_t *pSensor  = (sensor_t*)taskParmPtr;
	QueueHandle_t muestra;
	int32_t Led2;
	TickType_t xPeriodicity =  1000 / portTICK_RATE_MS;		// Tarea periodica cada 1000 ms
	TickType_t xLastWakeTime = xTaskGetTickCount();

	// ---------- REPETIR POR SIEMPRE --------------------------
	for(;;){

		if (xQueueReceive( muestra, &Led2, portMAX_DELAY )== pdTRUE){
			if(Led2 >= 300){
				gpioWrite( LED2 , 1 );
				vTaskDelay( 500 / portTICK_RATE_MS);
				gpioWrite( LED2 , 0 );
			}
		}else{
			vPrintString( "Could not receive from the queue.\r\n" );
		}
		vTaskDelayUntil( &xLastWakeTime , xPeriodicity );
	}
}


void PwmTask( void *taskParmPtr )
{
	// ---------- CONFIGURACIONES ------------------------------
	sensor_t *pSensor  = (sensor_t*)taskParmPtr;
	QueueHandle_t muestra;
	int32_t Pwm;
	uint8_t dutyCycle = 0;  /* 0 a 255 */
	pwmWrite( PWM7, dutyCycle );
	TickType_t xPeriodicity =  1000 / portTICK_RATE_MS;		// Tarea periodica cada 1000 ms
	TickType_t xLastWakeTime = xTaskGetTickCount();

	// ---------- REPETIR POR SIEMPRE --------------------------

	for(;;){

		if (xQueueReceive( muestra, &Pwm, portMAX_DELAY )== pdTRUE){
			if(Pwm >= 300 || Pwm <= 100){
				if( dutyCycle > 255 ){
					dutyCycle = 0;
				}
				pwmWrite( PWM7, dutyCycle );
				dutyCycle++;
				vTaskDelay( 50 / portTICK_RATE_MS );
			}
		}else{
			vPrintString( "Could not receive from the queue.\r\n" );
		}
		vTaskDelayUntil( &xLastWakeTime , xPeriodicity );

	}
}

void RegisterTask( void* taskParmPtr ){

	// ---------- CONFIGURACIONES ------------------------------
	sensor_t *pSensor  = (sensor_t*)taskParmPtr;
	QueueHandle_t muestra;
	int32_t Temp;
	char BufferRegistre[100];

	if (xQueueReceive( muestra, &Temp, portMAX_DELAY )== pdTRUE){
		if( f_open( &fp, FILENAME, FA_WRITE | FA_OPEN_APPEND ) == FR_OK ){
			UINT nbytes;
			UINT n = sprintf( BufferRegistre, "Temperature is: %3d °C \r\n",Temp);
			f_write( &fp, &pSensor ->bufferRegistre, n, &nbytes );

			f_close(&fp);

			if( nbytes == n ){
				vPrintString( "Escribio correctamente\r\n" );


			} else {

				printf("Escribio %d bytes\n", nbytes);
			}
		} else{
			vPrintString( "Error al abrir el archivo\r\n" );
		}
	}else{
				vPrintString( "Could not receive from the queue.\r\n" );
	}
	vTaskDelete(NULL);
}

/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/

