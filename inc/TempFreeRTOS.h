/*=============================================================================
 * Author: Freds Amundaray Cruz <freds.amundaray@gmail.com>
 * Date: 2020/06/07
 * Version: 1
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __TEMPFREERTOS_H__
#define __TEMPFREERTOS_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include <stdint.h>
#include <stddef.h>
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "sapi.h"
#include "userTasks.h"
#include "semphr.h"
#include "queue.h"
#include "supporting_functions.h"
#include "ff.h"       // <= Biblioteca FAT FS
#include "fssdc.h"	// API de bajo nivel para unidad "SDC:" en FAT FS

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/
QueueHandle_t muestraLed;
QueueHandle_t muestraLed2;
QueueHandle_t muestraPwm;
QueueHandle_t muestraRegistro;
SemaphoreHandle_t mutex;

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

/*=====[Prototypes (declarations) of public functions]=======================*/

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __TEMPFREERTOS_H__ */
