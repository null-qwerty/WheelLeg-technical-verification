#include "BaseControl/Controller/adrcController.hpp"
#include "tasks.hpp"

#include "BaseControl/Motor/RM3508.hpp"
#include "BaseControl/Controller/pidController.hpp"

void vTaskWheelReceive(void *pvParameters)
{
    wheelConnectivity.init();
    while (1) {
        // 接收 CAN2 中断发出的信号量
        // xClearCountOnExit 清空信号量数量
        // xTicksToWait 超时等待
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        // 解析 CAN 消息帧
        // clang-format off
        if (((CAN::xReceptionFrame_t*)wheelConnectivity.getReceiveFrame())->header.StdId == leftWheel.getReceiveId()) {
            leftWheel.decodeFeedbackMessage();
        } else if (((CAN::xReceptionFrame_t*)wheelConnectivity.getReceiveFrame())->header.StdId == rightWheel.getReceiveId()) {
            rightWheel.decodeFeedbackMessage();
        }
        // 向轮毂控制任务发出信号量，任务之间使用 xTaskNotifyGive()
        xTaskNotifyGive(wheelControlTaskHandle);
        // clang-format on
    }

    vTaskDelete(wheelReceiveTaskHandle);
}

pidController lwslc(20., 0.1, 1.2, 14000, -14000);
pidController rwslc(20., 0.1, 1.2, 14000, -14000);

void vTaskWheelControl(void *pvParameters)
{
    leftWheel.init();
    rightWheel.init();
    leftWheel.getSpeedLoopController() = &lwslc;
    rightWheel.getSpeedLoopController() = &rwslc;

    while (1) {
        // 接收从其他任务发出的信号量
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if (xSemaphoreTake(wheelControlMutex, 1)) {
            leftWheel.encodeControlMessage();
            rightWheel.encodeControlMessage();
            xSemaphoreGive(wheelControlMutex);
        }

        // 发送控制信息，若发送失败则重新初始化 CAN 线
        if (wheelConnectivity.sendMessage() != HAL_OK)
            wheelConnectivity.init();
    }

    vTaskDelete(wheelControlTaskHandle);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    // * 需要在中断中调用 HAL_CAN_GetRxMessage()，实测如果把
    // * 调用放到 notify 的任务中无法正常接收消息，还需调试
    wheelConnectivity.receiveMessage();
    // 判断接收的消息是否合法
    // clang-format off
    if (((CAN::xReceptionFrame_t*)(wheelConnectivity.getReceiveFrame()))->header.RTR != CAN_RTR_DATA ||
        ((CAN::xReceptionFrame_t*)(wheelConnectivity.getReceiveFrame()))->header.IDE != CAN_ID_STD ||
        ((CAN::xReceptionFrame_t*)(wheelConnectivity.getReceiveFrame()))->header.DLC != 8) {
        return;
    }
    // clang-format on
    // 高优先级优先，上下文切换时优先执行高优先级任务
    BaseType_t xHigherPriorityTaskWoken = pdTRUE;
    // 发送二进制信号量通知 wheelReceiveTask
    // 中断中需要调用 vTaskNotifyGiveFromISR()
    vTaskNotifyGiveFromISR(wheelReceiveTaskHandle, &xHigherPriorityTaskWoken);
    // 切换上下文
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    return;
}

xTaskHandle wheelReceiveTaskHandle;
xTaskHandle wheelControlTaskHandle;

CAN_FilterTypeDef wheelCanfilter = { .FilterIdHigh = 0x0000,
                                     .FilterIdLow = 0x0000,
                                     .FilterMaskIdHigh = 0x0000,
                                     .FilterMaskIdLow = 0x0000,
                                     .FilterFIFOAssignment = CAN_RX_FIFO0,
                                     .FilterBank = 14,
                                     .FilterMode = CAN_FILTERMODE_IDMASK,
                                     .FilterScale = CAN_FILTERSCALE_32BIT,
                                     .FilterActivation = CAN_FILTER_ENABLE,
                                     .SlaveStartFilterBank = 14 };
CAN wheelConnectivity = CAN(&hcan2, wheelCanfilter);

// pid 调参, 使用的 TI 的开源 pid，计算方式见 pidController::Calculate()
// 算法的微分部分似乎会造成高频震荡且微分输出值存在爆炸的风险，需查阅资料（解决，参考pidController::Calculate()）
RM3508 leftWheel = RM3508(wheelConnectivity, 2, 0x202, LEFT_MOTOR_CLOCKWISE);
RM3508 rightWheel = RM3508(wheelConnectivity, 1, 0x201, RIGHT_MOTOR_CLOCKWISE);