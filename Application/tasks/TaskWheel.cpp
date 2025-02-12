#include "tasks.hpp"

#include "BaseControl/Motor/RM3508.hpp"

void vTaskWheelReceive(void *pvParameters)
{
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
        // clang-format on
    }

    vTaskDelete(wheelReceiveTaskHandle);
}

void vTaskWheelControl(void *pvParameters)
{
    leftWheel.init();
    rightWheel.init();
    leftWheel.getSpeedLoopController()
        .setPidParam(20., 0.1, 1.2)
        .setOutLimit(14000, -14000);
    rightWheel.getSpeedLoopController()
        .setPidParam(20., 0.1, 1.2)
        .setOutLimit(14000, -14000);

    while (1) {
        // 接收从其他任务发出的信号量
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        leftWheel.encodeControlMessage();
        rightWheel.encodeControlMessage();

        // 发送控制信息，若发送失败则重新初始化 CAN 线
        if (wheelConnectivity.sendMessage() != HAL_OK)
            wheelConnectivity.init();
    }

    vTaskDelete(wheelControlTaskHandle);
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

// TODO pid 调参, 使用的 TI 的开源 pid，计算方式见 pidController::Calculate()
// TODO 算法的微分部分似乎会造成高频震荡且微分输出值存在爆炸的风险，需查阅资料
// TODO（似乎解决，参考 pidController::Calculate()）
RM3508 leftWheel = RM3508(wheelConnectivity, 2, 0x202, LEFT_MOTOR_CLOCKWISE);
RM3508 rightWheel = RM3508(wheelConnectivity, 1, 0x201, RIGHT_MOTOR_CLOCKWISE);