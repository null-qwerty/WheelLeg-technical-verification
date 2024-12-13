#include "cmsis_os2.h"

#include "WheelLeg/tasks.hpp"

void WheelLegTasksInit(void)
{
    /* 创建任务 */
    xTaskCreate(vTaskLED_G, "led_greenTask", 128 * 4, NULL, osPriorityNormal1,
                &xTaskLED_GHandle);
    xTaskCreate(vTaskWheelReceive, "wheelReceiveTask", 128 * 4, NULL,
                osPriorityAboveNormal, &wheelReceiveTaskHandle);
    xTaskCreate(vTaskWheelControl, "wheelControlTask", 128 * 4, NULL,
                osPriorityAboveNormal, &wheelControlTaskHandle);
    xTaskCreate(vTaskReadDbus, "readDbusTask", 128 * 4, NULL,
                osPriorityAboveNormal, &readDbusTaskHandle);
    xTaskCreate(vTaskJointControl, "jointControlTask", 128 * 4, NULL,
                osPriorityAboveNormal, &jointControlTaskHandle);
    xTaskCreate(vTaskJointReceive, "jointReceiveTask", 128 * 4, NULL,
                osPriorityAboveNormal, &jointReceiveTaskHandle);
}