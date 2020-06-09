/*
 * Temp.h
 *
 *  Created on: 8 jun. 2020
 *      Author: freds
 */

#ifndef MYPROGRAM_TEMPFREERTOS_INC_TEMP_H_
#define MYPROGRAM_TEMPFREERTOS_INC_TEMP_H_


#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "sapi.h"
#include "semphr.h"


typedef struct {
	char bufferRegistre;
	uint8_t Setting;
	uint8_t ValueAlarmHight;
	uint8_t ValueAlarmlow;
	int32_t AlarmHight;
	int32_t AlarmLow;
	int32_t Pwm;
	int32_t valor;
	QueueHandle_t muestra;
	QueueHandle_t Alarm_H;
	QueueHandle_t Alarm_L;
	QueueHandle_t PWM;
	QueueHandle_t Registro;
}sensor_t;

bool_t sensorInit(sensor_t * pSensor);
bool_t sensor_control(sensor_t * pSensor);

#endif /* MYPROGRAM_TEMPFREERTOS_INC_TEMP_H_ */
