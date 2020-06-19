/*=============================================================================
 * Author: Freds Amundaray Cruz <freds.amundaray@gmail.com>
 * Date: 2020/06/07
 * Version: 1
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "TempFreeRTOS.h"
#include "userTasks.h"


/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/


/*=====[Definitions of private global variables]=============================*/

DEBUG_PRINT_ENABLE;

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
	  // Inicializar y configurar la plataforma
		boardConfig();

		/* Configurar PWM */
		pwmConfig( 0, PWM_ENABLE );
		pwmConfig( PWM0, PWM_ENABLE_OUTPUT );
		// uartConfig( UART_USB, 115200 );
		adcConfig( ADC_ENABLE );
		// SPI configuration
		spiConfig( SPI0 );

		FSSDC_InitSPI ();
		FATFS fs;
		if( f_mount( &fs, "SDC:", 0 ) != FR_OK )
		{
			debugPrintlnString("\nError al crear el area de trabajo \n");//error
		}

		// UART for debug messages
		debugPrintConfigUart( UART_USB, 115200 );
		debugPrintlnString( "SensorTemperatura\n" );

		// Led para dar se�al de vida
		gpioWrite( LEDB , ON );

   // Create a task in freeRTOS with dynamic memory

	BaseType_t Led =
	xTaskCreate(
			Led_Task,            // Function that implements the task.
		(const char *)"Led Task",     // Text name for the task.
		configMINIMAL_STACK_SIZE*2, // Stack size in words, not bytes.
		0,                          // Parameter passed into the task.
		2,     		// Priority at which the task is created.
		0                           // Pointer to the task created in the system
	);

	if(Led == pdFAIL)
	{
		debugPrintlnString("\nError al crear la tarea \n");
	}

	BaseType_t Led2 =
	xTaskCreate(
			Led2_Task,            // Function that implements the task.
		(const char *)"Led2 Task",     // Text name for the task.
		configMINIMAL_STACK_SIZE*2, // Stack size in words, not bytes.
		0,                          // Parameter passed into the task.
		2,     		// Priority at which the task is created.
		0                           // Pointer to the task created in the system
	);

	if(Led2 == pdFAIL)
	{
		debugPrintlnString("\nError al crear la tarea \n");
	}
   BaseType_t Pwm =
   xTaskCreate(
		   PwmTask,                     // Function that implements the task.
      (const char *)"Pwm Task",     // Text name for the task.
      configMINIMAL_STACK_SIZE*2, // Stack size in words, not bytes.
      0,                          // Parameter passed into the task.
      tskIDLE_PRIORITY+2,         // Priority at which the task is created.
      0                           // Pointer to the task created in the system
   );

   if(Pwm == pdFAIL)
   {
	   debugPrintlnString("\nError al crear la tarea \n");
   }

   BaseType_t Muestra =
   xTaskCreate(
		   muestreoTask,             // Function that implements the task.
      (const char *)"muestreo Task",     // Text name for the task.
      configMINIMAL_STACK_SIZE*2, // Stack size in words, not bytes.
      0,                          // Parameter passed into the task.
	  1,  		 // Priority at which the task is created.
      0                           // Pointer to the task created in the system
   );

   if(Muestra == pdFAIL)
   {
	   debugPrintlnString("\nError al crear la tarea \n");
   }

   BaseType_t Registro =
   xTaskCreate(
		   RegisterTask,             // Function that implements the task.
		   (const char *)"Registro Task",     // Text name for the task.
		   configMINIMAL_STACK_SIZE*4, // Stack size in words, not bytes.
		   0,                          // Parameter passed into the task.
		   tskIDLE_PRIORITY+2,  		 // Priority at which the task is created.
		   0                           // Pointer to the task created in the system
   );

   if(Registro == pdFAIL)
   {
	   debugPrintlnString("\nError al crear la tarea \n");
   }

   BaseType_t Disk =
   xTaskCreate(
		   DiskTask,
		   (const char *)"DiskTask",
		   configMINIMAL_STACK_SIZE*2,
		   0,
		   tskIDLE_PRIORITY+2,
		   0
   );

   if(Disk == pdFAIL)
   {
	   debugPrintlnString("\nError al crear la tarea \n");
   }


   muestraLed = xQueueCreate( 20, sizeof( uint16_t ) );

	if( muestraLed == NULL ) {
		debugPrintlnString("Error al crear la cola muestraLed2\n");
	}

	muestraLed2 = xQueueCreate( 20, sizeof( uint16_t ) );

	if( muestraLed2 == NULL ) {
		debugPrintlnString("Error al crear la cola muestraLed2\n");
	}

	muestraPwm = xQueueCreate( 20, sizeof( uint16_t ) );

	if( muestraPwm == NULL ) {
		debugPrintlnString("Error al crear la cola muestraPwm\n");
	}

	muestraRegistro = xQueueCreate( 20, sizeof( uint16_t ) );

	if( muestraRegistro == NULL ) {
		debugPrintlnString("Error al crear la cola muestraRegistro\n");
	}

	mutex 	=  xSemaphoreCreateMutex();

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



