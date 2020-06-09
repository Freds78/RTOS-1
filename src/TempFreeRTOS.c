/*=============================================================================
 * Author: Freds Amundaray Cruz <freds.amundaray@gmail.com>
 * Date: 2020/06/07
 * Version: 1
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "TempFreeRTOS.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "sapi.h"
#include "userTasks.h"
#include "Temp.h"
#include "semphr.h"
#include "queue.h"
#include "supporting_functions.h"
#include "ff.h"       // <= Biblioteca FAT FS
#include "fssdc.h"	// API de bajo nivel para unidad "SDC:" en FAT FS

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/
#define FILENAME "SDC:/Temperature.txt"
DEBUG_PRINT_ENABLE;
static  FIL fp;


sensor_t pSensor;
/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
	  // Inicializar y configurar la plataforma
		boardConfig();
		/* Configurar PWM */
		pwmConfig( 0, PWM_ENABLE );
		pwmConfig( PWM7, PWM_ENABLE_OUTPUT );
		// uartConfig( UART_USB, 115200 );
		adcConfig( ADC_ENABLE );
		// SPI configuration
		spiConfig( SPI0 );

		FSSDC_InitSPI ();
		FATFS fs;
		if( f_mount( &fs, "SDC:", 0 ) != FR_OK )
		{
			debugPrintlnString("Error al crear el area de trabajo \n");//error
		}

		// UART for debug messages
		debugPrintConfigUart( UART_USB, 115200 );
		debugPrintlnString( "SensorTemperatura\n" );

		// Led para dar se�al de vida
		gpioWrite( LEDB , ON );
/*
   // Create a task in freeRTOS with dynamic memory
   xTaskCreate(
      myTask,                     // Function that implements the task.
      (const char *)"myTask",     // Text name for the task.
      configMINIMAL_STACK_SIZE*2, // Stack size in words, not bytes.
      0,                          // Parameter passed into the task.
      tskIDLE_PRIORITY+1,         // Priority at which the task is created.
      0                           // Pointer to the task created in the system
   );
*/
		pSensor.Alarm_H = xQueueCreate( 5, sizeof( sensor_t ) );

		if( pSensor.Alarm_H == NULL ) {
			debugPrintlnString("Error al crear la tarea Alarma alta\n");
		}

		pSensor.Alarm_L = xQueueCreate( 5, sizeof( sensor_t ) );

		if( pSensor.Alarm_L == NULL ) {
			debugPrintlnString("Error al crear la tarea Alarma baja\n");
		}

		pSensor.PWM = xQueueCreate( 5, sizeof( sensor_t ) );

		if( pSensor.PWM == NULL ) {
			debugPrintlnString("Error al crear la tarea PWM\n");
		}

		pSensor.muestra = xQueueCreate( 5, sizeof( sensor_t ) );

		if( pSensor.muestra == NULL ) {
			debugPrintlnString("Error al crear la tarea muestra\n");
		}

		pSensor.Registro = xQueueCreate( 5, sizeof( sensor_t ) );

		if( pSensor.Registro == NULL ) {
			debugPrintlnString("Error al crear la tarea Registro\n");
		}

   vTaskStartScheduler(); // Initialize scheduler

   while( TRUE){

	   // If reach heare it means that the scheduler could not start
   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}
//____________________________________________________________________


