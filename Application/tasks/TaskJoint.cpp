#include "BaseControl/Controller/classicController.hpp"
#include "BaseControl/Controller/pidController.hpp"
#include "tasks.hpp"

#include "BaseControl/Motor/DM4310.hpp"
#include "cmsis_os2.h"
#include "portmacro.h"
#include "projdefs.h"

void vTaskJointInit(void *pvParameters)
{
    jointConnectivity.init();
    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        jointInited = true;

        auto initPidController = [](DM4310 &motor) -> void {
            pidController *sl = (pidController *)motor.getSpeedLoopController();
            pidController *al = (pidController *)motor.getAngleLoopController();
            sl->setPidParam(0.326, 0., 0).setOutLimit(9.8, -9.8).init();
            al->setPidParam(30., 0.001, 15).setOutLimit(30, -30).init();
        };

        leftFrontJoint.init();
        initPidController(leftFrontJoint);
        // leftFrontJoint.getTargetState().position = 1.0;
        osDelay(1);
        leftBackJoint.init();
        initPidController(leftBackJoint);
        // leftBackJoint.getTargetState().position = 1.0;
        osDelay(1);
        rightFrontJoint.init();
        initPidController(rightFrontJoint);
        // rightFrontJoint.getTargetState().position = 1.0;
        osDelay(1);
        rightBackJoint.init();
        initPidController(rightBackJoint);
        // rightBackJoint.getTargetState().position = 1.0;
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

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    // * 需要在中断中调用 HAL_CAN_GetRxMessage()，实测如果把
    // * 调用放到 notify 的任务中无法正常接收消息，还需调试
    jointConnectivity.receiveMessage();
    // 判断接收的消息是否合法
    // clang-format off
    if (((CAN::xReceptionFrame_t*)(jointConnectivity.getReceiveFrame()))->header.RTR != CAN_RTR_DATA ||
        ((CAN::xReceptionFrame_t*)(jointConnectivity.getReceiveFrame()))->header.IDE != CAN_ID_STD ||
        ((CAN::xReceptionFrame_t*)(jointConnectivity.getReceiveFrame()))->header.DLC != 8) {
        return;
    }
    // clang-format on
    if (((CAN::xReceptionFrame_t *)jointConnectivity.getReceiveFrame())
            ->header.StdId == leftFrontJoint.getReceiveId())
        leftFrontJoint.decodeFeedbackMessage();
    else if (((CAN::xReceptionFrame_t *)jointConnectivity.getReceiveFrame())
                 ->header.StdId == leftBackJoint.getReceiveId())
        leftBackJoint.decodeFeedbackMessage();
    else if (((CAN::xReceptionFrame_t *)jointConnectivity.getReceiveFrame())
                 ->header.StdId == rightFrontJoint.getReceiveId())
        rightFrontJoint.decodeFeedbackMessage();
    else if (((CAN::xReceptionFrame_t *)jointConnectivity.getReceiveFrame())
                 ->header.StdId == rightBackJoint.getReceiveId())
        rightBackJoint.decodeFeedbackMessage();
    // 高优先级优先，上下文切换时优先执行高优先级任务
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // 发送二进制信号量通知 wheelReceiveTask
    // 中断中需要调用 vTaskNotifyGiveFromISR()
    vTaskNotifyGiveFromISR(jointControlTaskHandle, &xHigherPriorityTaskWoken);
    // 切换上下文
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    return;
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