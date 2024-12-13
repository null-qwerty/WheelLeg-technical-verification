#include "WheelLeg/tasks.hpp"

#include "PHOENIX/BaseControl/Motor/DM4310.hpp"
#include "cmsis_os2.h"
#include "portmacro.h"
#include "projdefs.h"
#include "stm32f4xx_hal.h"

void vTaskJointControl(void *pvParameters)
{
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    leftFrontJoint.init();
    leftFrontJoint.getSpeedLoopController()
        .setPidParam(0.326, 0., 0)
        .setOutLimit(9.8, -9.8);
    leftFrontJoint.getAngleLoopController()
        .setPidParam(30., 0.001, 15)
        .setOutLimit(30, -30);
    leftFrontJoint.getTargetState().position = 1.0;
    osDelay(1);
    leftBackJoint.init();
    osDelay(1);
    rightFrontJoint.init();
    osDelay(1);
    rightBackJoint.init();
    // 定频执行任务
    TickType_t xLastWakeTime;
    TickType_t xFrequency = pdMS_TO_TICKS(1);
    xLastWakeTime = xTaskGetTickCount();
    while (1) {
        leftFrontJoint.encodeControlMessage();
        if (jointConnectivity.sendMessage() != HAL_OK)
            jointConnectivity.init();
        // HAL_Delay(1);
        // leftBackJoint.encodeControlMessage();
        // if (jointConnectivity.sendMessage() != HAL_OK)
        //     jointConnectivity.init();
        // HAL_Delay(1);
        // rightFrontJoint.encodeControlMessage();
        // if (jointConnectivity.sendMessage() != HAL_OK)
        //     jointConnectivity.init();
        // HAL_Delay(1);
        // rightBackJoint.encodeControlMessage();
        // if (jointConnectivity.sendMessage() != HAL_OK)
        //     jointConnectivity.init();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
    vTaskDelete(jointControlTaskHandle);
}

void vTaskJointReceive(void *pvParameters)
{
    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        // clang-format off
        if (((CAN_RxHeaderTypeDef *)jointConnectivity.getRxHeader())->StdId == leftFrontJoint.getReceiveId())
            leftFrontJoint.decodeFeedbackMessage();
        else if (((CAN_RxHeaderTypeDef *)jointConnectivity.getRxHeader())->StdId == leftBackJoint.getReceiveId())
            leftBackJoint.decodeFeedbackMessage();
        else if (((CAN_RxHeaderTypeDef *)jointConnectivity.getRxHeader())->StdId == rightFrontJoint.getReceiveId())
            rightFrontJoint.decodeFeedbackMessage();
        else if (((CAN_RxHeaderTypeDef *)jointConnectivity.getRxHeader())->StdId == rightBackJoint.getReceiveId())
            rightBackJoint.decodeFeedbackMessage();
        // clang-format on
    }
    vTaskDelete(jointReceiveTaskHandle);
}

xTaskHandle jointControlTaskHandle;
xTaskHandle jointReceiveTaskHandle;

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
