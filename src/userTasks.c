/*=============================================================================
 * Author: Freds Amundaray Cruz <freds.amundaray@gmail.com>
 * Date: 2020/06/07
 * Version: 1
 *===========================================================================*/

/*=====[Inclusion of own header]=============================================*/

#include "userTasks.h"
#include "TempFreeRTOS.h"


/*=====[Inclusions of private function dependencies]=========================*/

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/
#define FILENAME "SDC:/TempRTOS.txt"
static  FIL fp;

/*=====[Implementations of public functions]=================================*/

// Task implementation

void muestreoTask( void* taskParmPtr ){

	// ---------- CONFIGURACIONES ------------------------------

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

		xSemaphoreTake( mutex , portMAX_DELAY );
		xQueueSendToBack(muestraLed , &Temperatura,  portMAX_DELAY );
		xSemaphoreGive( mutex );

		xSemaphoreTake( mutex , portMAX_DELAY );
		xQueueSendToBack(muestraLed2 , &Temperatura,  portMAX_DELAY );
		xSemaphoreGive( mutex );

		xSemaphoreTake( mutex , portMAX_DELAY );
		xQueueSendToBack(muestraPwm , &Temperatura,  portMAX_DELAY );
		xSemaphoreGive( mutex );

		xSemaphoreTake( mutex , portMAX_DELAY );
		xQueueSend(muestraRegistro , &Temperatura,  portMAX_DELAY );
		xSemaphoreGive( mutex );


		 vTaskDelay( 1000 / portTICK_RATE_MS );
	}

}


void Led_Task( void *taskParmPtr )
{
	// ---------- CONFIGURACIONES ------------------------------


	uint16_t Led;
	Led = 0;
	BaseType_t xStatus;
	// ---------- REPETIR POR SIEMPRE --------------------------
	for(;;){

		if( uxQueueMessagesWaiting( muestraLed ) != 0 ) {
			vPrintString( "Queue should have been empty!\r\n" );
		}
		xStatus = xQueueReceive( muestraLed, &Led, portMAX_DELAY );

		if( xStatus == pdPASS ){
			if(Led > 300 && Led < 350){
				gpioWrite( LED1 , ON );
				vTaskDelay( 800 / portTICK_RATE_MS);
				gpioWrite( LED1 , OFF );
			}
			if(Led < 100 && Led > 50){
				gpioWrite( LED1 , ON );
				vTaskDelay( 800 / portTICK_RATE_MS);
				gpioWrite( LED1 , OFF );
			}
		}else {

			vPrintString( "Could not receive from the queue.\r\n" );
		}
		vTaskDelay( 1000 / portTICK_RATE_MS );
	}
}

void Led2_Task( void *taskParmPtr )
{
	// ---------- CONFIGURACIONES ------------------------------
	uint16_t Led2;
	Led2 = 0;
	BaseType_t xStatus;
	// ---------- REPETIR POR SIEMPRE --------------------------
	for(;;){

		if( uxQueueMessagesWaiting( muestraLed2 ) != 0 ) {
			vPrintString( "Queue should have been empty!\r\n" );
		}
		xStatus = xQueueReceive( muestraLed2, &Led2, portMAX_DELAY );

		if( xStatus == pdPASS ){
			if(Led2 > 350 || Led2 < 50){
				gpioWrite( LED2 , ON );
				vTaskDelay( 300 / portTICK_RATE_MS);
				gpioWrite( LED2 , OFF );
			}
		}else {

			vPrintString( "Could not receive from the queue.\r\n" );
		}
		vTaskDelay( 1000 / portTICK_RATE_MS );
	}
}

void PwmTask( void *taskParmPtr )
{
	// ---------- CONFIGURACIONES ------------------------------

	uint16_t Pwm;
	Pwm = 0;
	BaseType_t xStatus;
	uint8_t dutyCycle = 0;  /* 0 a 255 */
	pwmWrite( PWM0, dutyCycle );

	// ---------- REPETIR POR SIEMPRE --------------------------

	for(;;){

		if( uxQueueMessagesWaiting( muestraPwm ) != 0 ) {
			vPrintString( "Queue should have been empty!\r\n" );
		}
		xStatus = xQueueReceive( muestraPwm, &Pwm, portMAX_DELAY);

		if( xStatus == pdPASS ){
			if (Pwm > 300 && Pwm < 350){
				dutyCycle = 100;
				pwmWrite( PWM0, dutyCycle );
			}else if(Pwm < 100 && Pwm > 50){
				dutyCycle = 100;
				pwmWrite( PWM0, dutyCycle );
			}else if(Pwm > 350 && Pwm <= 400){
				dutyCycle = 200;
				pwmWrite( PWM0, dutyCycle );
			}else if(Pwm < 50 && Pwm >= 0){
				dutyCycle = 200;
				pwmWrite( PWM0, dutyCycle );
			}else{
				dutyCycle = 0;
				pwmWrite( PWM0, dutyCycle );
			}
		}else {
			vPrintString( "Could not receive from the queue.\r\n" );
		}

		vTaskDelay( 1000 / portTICK_RATE_MS );

	}
}

void DiskTask(void *taskParmPtr)
{

	portTickType xPeriodicity = 10 / portTICK_RATE_MS;
	portTickType xLastWakeTime = xTaskGetTickCount();

	for (;;)
	{
		disk_timerproc();
		vTaskDelayUntil(&xLastWakeTime, xPeriodicity);
	}
}

void RegisterTask( void* taskParmPtr ){

	// ---------- CONFIGURACIONES ------------------------------
	uint16_t Temp;
	Temp = 0;
	uint8_t BufferRegistre[200];
	uint8_t valor;
	BaseType_t xStatus;

	// ---------- REPETIR POR SIEMPRE --------------------------

	for(;;){
		if( uxQueueMessagesWaiting( muestraRegistro ) != 0 ) {
			vPrintString( "Queue should have been empty!\r\n" );
		}

		xStatus = xQueueReceive( muestraRegistro, &Temp, portMAX_DELAY );
		valor = (uint16_t)Temp;
		if( xStatus == pdPASS ){
			if( f_open( &fp, FILENAME, FA_WRITE | FA_OPEN_APPEND ) == FR_OK ){
				UINT nbytes;
				UINT n = sprintf( BufferRegistre, "Temperature is: %d °C \r\n",Temp);
				f_write( &fp, BufferRegistre, n, &nbytes );

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
		vTaskDelay( 1000 / portTICK_RATE_MS );
	}

}

/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/

