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
/*=====[Inclusions of private function dependencies]=========================*/

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/
void vPwmTask( void *pvParameters );
void vmuestreoTask( void *pvParameters );
void vLed1_LowTask( void *pvParameters );
void vLed2_LowTask( void *pvParameters );

/*=====[Implementations of public functions]=================================*/

// Task implementation

void vmuestreoTask( void *pvParameters ){
	// ---------- CONFIGURACIONES ------------------------------
	sensor_t *pSensor  = (sensor_t*)pvParameters;
	int32_t lReceivedValue;
	// ---------- REPETIR POR SIEMPRE --------------------------
	for( ;; )
	{

		if( uxQueueMessagesWaiting(pSensor->muestra ) != 0 ) {
			vPrintString( "Queue should have been empty!\r\n" );
		}

		if( xQueueReceive( pSensor->muestra, &lReceivedValue, portMAX_DELAY  ) != pdPASS ) //portMAX_DELAY
		{

			vPrintStringAndNumber( "Temperature is:", lReceivedValue );
		}
		else
		{
			/* xMessage now contains the received data. */
		vPrintString( "Could not receive from the queue.\r\n" );
		}

		vTaskDelay( 10 / portTICK_RATE_MS);
	}
}

void vLed1_LowTask( void *pvParameters )
{
   /* Declare the variable that will hold the values received from the queue. */
	sensor_t *pSensor  = (sensor_t*)pvParameters;
	int32_t lReceivedValueLed1;
   BaseType_t xStatus;
  // const TickType_t xTicksToWait = pdMS_TO_TICKS( 100UL );

   /* This task is also defined within an infinite loop. */
   for( ;; ) {

      if( uxQueueMessagesWaiting( pSensor->Alarm_L ) != 0 ) {
         vPrintString( "Queue should have been empty!\r\n" );
      }

      xStatus = xQueueReceive( pSensor->Alarm_L, &lReceivedValueLed1, portMAX_DELAY );

      if( xStatus == pdPASS ) {

    	  	if(lReceivedValueLed1 == 1){
    	  		gpioWrite( LED1 , 1 );
    	  		vTaskDelay( 100 / portTICK_RATE_MS);
    	  		gpioWrite( LED1 , 0 );
    	  	}

      } else {
         vPrintString( "Could not receive from the queue.\r\n" );
      }
   }
}

void vLed2_LowTask( void *pvParameters )
{
   /* Declare the variable that will hold the values received from the queue. */
	sensor_t *pSensor  = (sensor_t*)pvParameters;
	int32_t lReceivedValueLed2;
   BaseType_t xStatus;
  // const TickType_t xTicksToWait = pdMS_TO_TICKS( 100UL );

   /* This task is also defined within an infinite loop. */
   for( ;; ) {

      if( uxQueueMessagesWaiting( pSensor->Alarm_H ) != 0 ) {
         vPrintString( "Queue should have been empty!\r\n" );
      }

      xStatus = xQueueReceive( pSensor->Alarm_H, &lReceivedValueLed2, portMAX_DELAY );

      if( xStatus == pdPASS ) {

    	  	if(lReceivedValueLed2 == 1){
    	  		gpioWrite( LED2 , 1 );
    	  		vTaskDelay( 100 / portTICK_RATE_MS);
    	  		gpioWrite( LED2 , 0 );
    	  	}

      } else {
         vPrintString( "Could not receive from the queue.\r\n" );
      }
   }
}

void vPwmTask( void *pvParameters )
{
   /* Declare the variable that will hold the values received from the queue. */
	sensor_t *pSensor  = (sensor_t*)pvParameters;
	int32_t lReceivedValuePwm;
	BaseType_t xStatus;
	uint8_t dutyCycle = 0;  /* 0 a 255 */
	pwmWrite( PWM7, dutyCycle );

   /* This task is also defined within an infinite loop. */
   for( ;; ) {

      if( uxQueueMessagesWaiting( pSensor->PWM ) != 0 ) {
         vPrintString( "Queue should have been empty!\r\n" );
      }

      xStatus = xQueueReceive( pSensor->PWM, &lReceivedValuePwm, portMAX_DELAY );

      if( xStatus == pdPASS ) {

    	  	if(lReceivedValuePwm == 1 ){
       			if( dutyCycle > 255 ){
       				dutyCycle = 0;
       			}
       			pwmWrite( PWM7, dutyCycle );
       			dutyCycle++;
       			vTaskDelay( 50 / portTICK_RATE_MS );
    	  	}

      } else {
         vPrintString( "Could not receive from the queue.\r\n" );
      }
   }
}

/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/

