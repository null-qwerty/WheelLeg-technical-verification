#include "cmsis_os2.h"

#include "tasks.hpp"

void WheelLegTasksInit(void)
{
    /* 创建任务 */
    xTaskCreate(vTaskLED_G, "led_greenTask", 128 * 2, NULL, osPriorityNormal1,
                &xTaskLED_GHandle);
    xTaskCreate(vTaskWheelReceive, "wheelReceiveTask", 128 * 2, NULL,
                osPriorityAboveNormal, &wheelReceiveTaskHandle);
    xTaskCreate(vTaskWheelControl, "wheelControlTask", 128 * 2, NULL,
                osPriorityAboveNormal, &wheelControlTaskHandle);
    xTaskCreate(vTaskReadDbus, "readDbusTask", 128 * 2, NULL,
                osPriorityAboveNormal, &readDbusTaskHandle);
    // xTaskCreate(vTaskJointControl, "jointControlTask", 128 * 2, NULL,
    //             osPriorityAboveNormal, &jointControlTaskHandle);
    // xTaskCreate(vTaskJointInit, "jointInitTask", 128 * 2, NULL,
    //             osPriorityAboveNormal, &jointInitTaskHandle);
    // xTaskCreate(vTaskJointDeinit, "jointDeinitTask", 128 * 2, NULL,
    //             osPriorityAboveNormal, &jointDeinitTaskHandle);
    xTaskCreate(vTaskSensor, "sensorTask", 128 * 4, NULL, osPriorityAboveNormal,
                &sensorTaskHandle);
    xTaskCreate(vTaskTemperatureHold, "temperatureHoldTask", 128 * 2, NULL,
                osPriorityAboveNormal, &temperatureHoldTaskHandle);
}