#include "tasks.hpp"

#include "BaseControl/Motor/DM4310.hpp"
#include "cmsis_os2.h"
#include "portmacro.h"
#include "projdefs.h"

void vTaskJointInit(void *pvParameters)
{
    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        jointInited = true;

        leftFrontJoint.init();
        leftFrontJoint.getSpeedLoopController()
            .setPidParam(0.326, 0., 0)
            .setOutLimit(9.8, -9.8)
            .init();
        leftFrontJoint.getAngleLoopController()
            .setPidParam(30., 0.001, 15)
            .setOutLimit(30, -30)
            .init();
        leftFrontJoint.getTargetState().position = 1.0;
        osDelay(1);
        leftBackJoint.init();
        leftBackJoint.getSpeedLoopController()
            .setPidParam(0.326, 0., 0)
            .setOutLimit(9.8, -9.8)
            .init();
        leftBackJoint.getAngleLoopController()
            .setPidParam(30., 0.001, 15)
            .setOutLimit(30, -30)
            .init();
        leftBackJoint.getTargetState().position = 1.0;
        osDelay(1);
        rightFrontJoint.init();
        rightFrontJoint.getSpeedLoopController()
            .setPidParam(0.326, 0., 0)
            .setOutLimit(9.8, -9.8)
            .init();
        rightFrontJoint.getAngleLoopController()
            .setPidParam(30., 0.001, 15)
            .setOutLimit(30, -30)
            .init();
        rightFrontJoint.getTargetState().position = 1.0;
        osDelay(1);
        rightBackJoint.init();
        rightBackJoint.getSpeedLoopController()
            .setPidParam(0.326, 0., 0)
            .setOutLimit(9.8, -9.8)
            .init();
        rightBackJoint.getAngleLoopController()
            .setPidParam(30., 0.001, 15)
            .setOutLimit(30, -30)
            .init();
        rightBackJoint.getTargetState().position = 1.0;
    }
    vTaskDelete(jointInitTaskHandle);
}

void vTaskJointDeinit(void *pvParameters)
{
    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        jointInited = false;
        leftFrontJoint.deInit();
        osDelay(1);
        leftBackJoint.deInit();
        osDelay(1);
        rightFrontJoint.deInit();
        osDelay(1);
        rightBackJoint.deInit();
    }
    vTaskDelete(jointDeinitTaskHandle);
}

void vTaskJointControl(void *pvParameters)
{
    // 定频执行任务
    TickType_t xLastWakeTime;
    TickType_t xFrequency = pdMS_TO_TICKS(2);
    xLastWakeTime = xTaskGetTickCount();
    while (1) {
        leftFrontJoint.encodeControlMessage();
        jointConnectivity.sendMessage();
        ulTaskNotifyTake(pdTRUE, 5);
        leftBackJoint.encodeControlMessage();
        jointConnectivity.sendMessage();
        ulTaskNotifyTake(pdTRUE, 5);
        rightFrontJoint.encodeControlMessage();
        jointConnectivity.sendMessage();
        ulTaskNotifyTake(pdTRUE, 5);
        rightBackJoint.encodeControlMessage();
        jointConnectivity.sendMessage();
        ulTaskNotifyTake(pdTRUE, 5);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
    vTaskDelete(jointControlTaskHandle);
}

xTaskHandle jointInitTaskHandle;
xTaskHandle jointDeinitTaskHandle;
xTaskHandle jointControlTaskHandle;

CAN_FilterTypeDef jointCanFilter = { .FilterIdHigh = 0x0000,
                                     .FilterIdLow = 0x0000,
                                     .FilterMaskIdHigh = 0x0000,
                                     .FilterMaskIdLow = 0x0000,
                                     .FilterFIFOAssignment = CAN_RX_FIFO1,
                                     .FilterBank = 0,
                                     .FilterMode = CAN_FILTERMODE_IDMASK,
                                     .FilterScale = CAN_FILTERSCALE_32BIT,
                                     .FilterActivation = CAN_FILTER_ENABLE,
                                     .SlaveStartFilterBank = 14 };

CAN jointConnectivity = CAN(&hcan1, jointCanFilter);

// clang-format off
DM4310 leftFrontJoint = DM4310(jointConnectivity, 0x07, 0x17,
                                   LEFT_MOTOR_CLOCKWISE *FRONT_MOTOR_CLOCKWISE);
DM4310 leftBackJoint = DM4310(jointConnectivity, 0x08, 0x18,
                                  LEFT_MOTOR_CLOCKWISE *BACK_MOTOR_CLOCKWISE);
DM4310 rightFrontJoint = DM4310(jointConnectivity, 0x06, 0x16,
               RIGHT_MOTOR_CLOCKWISE *FRONT_MOTOR_CLOCKWISE);
DM4310 rightBackJoint = DM4310(jointConnectivity, 0x05, 0x15,
                                   RIGHT_MOTOR_CLOCKWISE *BACK_MOTOR_CLOCKWISE);
// clang-format on
bool jointInited = false;