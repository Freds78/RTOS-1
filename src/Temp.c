/*
 * Temp.c
 *
 *  Created on: 8 jun. 2020
 *      Author: freds
 */


#include "Temp.h"
#include "sapi.h"
#include "FreeRTOS.h"
#include "semphr.h"



bool_t sensorInit(sensor_t * pSensor){

	if (NULL == pSensor){
	return FALSE;
	}
	pSensor->ValueAlarmlow = 50;
	pSensor->ValueAlarmHight = 150;
	pSensor->AlarmLow = 0;
	pSensor->AlarmHight = 0;
	pSensor-> Pwm = 0;
	return TRUE;

}


bool_t sensor_control(sensor_t * pSensor){

	if (NULL == pSensor){
	      return FALSE;
	}

	uint16_t ValorADC = 0;
	uint16_t Temperatura = 0;

	ValorADC = adcRead( CH1 );
	Temperatura = (400*ValorADC)/1024;
	pSensor-> valor = (uint16_t)Temperatura;

	//xQueueSend( pSensor->muestra , &pSensor->valor,  portMAX_DELAY  );
	//xQueueSend( pSensor->Registro , &pSensor->valor,  portMAX_DELAY  );

	if(pSensor->ValueAlarmlow >= Temperatura){
		pSensor->AlarmLow = 1;
		xQueueSend( pSensor->Alarm_L , &pSensor->AlarmLow,  portMAX_DELAY  );
	}

	if(pSensor->ValueAlarmHight <= Temperatura){
		pSensor-> AlarmHight = 1;
		xQueueSend( pSensor->Alarm_H , &pSensor->AlarmHight,  portMAX_DELAY  );
	}

	if(pSensor->ValueAlarmlow >= Temperatura ||
			pSensor->ValueAlarmHight <= Temperatura ){
		pSensor->Pwm = 1;
		xQueueSend( pSensor->PWM , &pSensor->Pwm,  portMAX_DELAY  );
	}




	return TRUE;


}




