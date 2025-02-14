#include "usart.h"

#include "tasks.hpp"

void vTaskReadDbus(void *pvParameters)
{
    // 当前未实现关节电机控制，故将直接控制轮毂电机运动
    // 后续运动控制可能会通过调整重心的变化实现
    dbus.init();
    TickType_t xLastWakeTime;
    TickType_t xFrequency = pdMS_TO_TICKS(2);
    xLastWakeTime = xTaskGetTickCount();
    while (1) {
        // 读取遥控器数据
        if (dbus.receiveMessage() == HAL_OK) {
            dbus.decodeDBUSMessage();
        }

        if (xSemaphoreTake(wheelControlMutex, 1)) {
            // 前进
            leftWheel.getTargetState().velocity =
                (dbus.getDBUSData().rc.ch1 - 1024) / 660.0 * 1800;
            rightWheel.getTargetState().velocity =
                leftWheel.getTargetState().velocity;
            // 转向
            leftWheel.getTargetState().velocity +=
                (dbus.getDBUSData().rc.ch0 - 1024) / 660.0 * 1000;
            rightWheel.getTargetState().velocity -=
                (dbus.getDBUSData().rc.ch0 - 1024) / 660.0 * 1000;

            xSemaphoreGive(wheelControlMutex);
        }

        // 向轮毂控制任务发出信号量，任务之间使用 xTaskNotifyGive()
        // xTaskNotifyGive(wheelControlTaskHandle);
        // 向关节控制任务发出信号量
        if (!jointInited && dbus.getDBUSData().rc.s2 == 3)
            xTaskNotifyGive(jointInitTaskHandle);
        else if (jointInited && dbus.getDBUSData().rc.s2 == 1)
            xTaskNotifyGive(jointDeinitTaskHandle);

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }

    vTaskDelete(readDbusTaskHandle);
}

xTaskHandle readDbusTaskHandle;

SemaphoreHandle_t wheelControlMutex;

DBUS dbus(&huart3, DBUS::RX);
