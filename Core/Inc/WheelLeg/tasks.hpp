#pragma once

#include "FreeRTOS.h"
#include "PHOENIX/BaseControl/Connectivity/CAN/CAN.hpp"
#include "cmsis_os2.h"
#include "task.h"

#include "PHOENIX/BaseControl/Motor/RM3508.hpp"
#include "PHOENIX/BaseControl/Motor/DM4310.hpp"
#include "PHOENIX/BaseControl/Connectivity/UART/DBUS.hpp"

void WheelLegTasksInit(void);

/**
 * @brief LED 闪烁绿灯，用于表示系统正常运行，调度器正常工作
 *
 * @param pvParameters 任务参数，未使用
 *
 * @note 任务优先级仅高于默认任务。正常情况下绿灯闪烁频率为 1Hz
 * (osPriorityNormal1)
 */
__weak void vTaskLED_G(void *pvParameters);

/**
 * @brief 接收轮毂的电机的反馈信号
 *
 * @param pvParameters 任务参数，未使用
 *
 * @note 以 1000Hz 的频率接收电机的反馈信号，将反馈值存入全局变量中
 * (osPriorityAboveNormal)
 */
__weak void vTaskWheelReceive(void *pvParameters);

/**
 * @brief 向轮毂的电机发送控制信号
 *
 * @param pvParameters 任务参数，未使用
 *
 * @note 通过 notify 执行 (osPriorityAboveNormal)
 */
__weak void vTaskWheelControl(void *pvParameters);

/**
 * @brief 读取遥控器的数据
 *
 * @param pvParameters 任务参数，未使用
 *
 * @note 以 1000 Hz 的频率读取 DBUS 数据 (osPriorityAboveNormal)
 */
__weak void vTaskReadDbus(void *pvParameters);

__weak void vTaskJointControl(void *pvParameters);

__weak void vTaskJointReceive(void *pvParameters);

extern xTaskHandle xTaskLED_GHandle;
extern xTaskHandle wheelReceiveTaskHandle;
extern xTaskHandle wheelControlTaskHandle;
extern xTaskHandle readDbusTaskHandle;
extern xTaskHandle jointControlTaskHandle;
extern xTaskHandle jointReceiveTaskHandle;

extern CAN_FilterTypeDef wheelCanfilter;
extern CAN wheelConnectivity;
// TODO pid 调参, 使用的 TI 的开源 pid，计算方式见 pidController::Calculate()
// TODO 算法的微分部分似乎会造成高频震荡且微分输出值存在爆炸的风险，需查阅资料
// TODO（似乎解决，参考 pidController::Calculate()）
extern RM3508 leftWheel, rightWheel;

extern DBUS dbus;

extern CAN_FilterTypeDef jheelCanfilter;
extern CAN jointConnectivity;
extern DM4310 leftFrontJoint, leftBackJoint, rightFrontJoint, rightBackJoint;
