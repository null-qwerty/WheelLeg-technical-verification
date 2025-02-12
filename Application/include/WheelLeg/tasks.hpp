#pragma once

#include "main.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "task.h"

#include "PHOENIX/BaseControl/Connectivity/CAN/CAN.hpp"
#include "PHOENIX/BaseControl/Connectivity/UART/DBUS.hpp"
#include "PHOENIX/BaseControl/Motor/RM3508.hpp"
#include "PHOENIX/BaseControl/Motor/DM4310.hpp"
#include "PHOENIX/BaseControl/Sensor/BMI088/BMI088.hpp"
#include "PHOENIX/BaseControl/Connectivity/SPI/SPI.hpp"

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
 * @note 以 200 Hz 的频率读取 DBUS 数据 (osPriorityAboveNormal)
 */
__weak void vTaskReadDbus(void *pvParameters);
/**
 * @brief 关节电机使能
 *
 * @param pvParameters 任务参数，未使用
 * @note 通过 DBUS 发送的信号量触发 (osPriorityAboveNormal)
 */
__weak void vTaskJointInit(void *pvParameters);
/**
 * @brief 关节电机失能
 *
 * @param pvParameters 任务参数，未使用
 * @note 通过 DBUS 发送的信号量触发 (osPriorityAboveNormal)
 */
__weak void vTaskJointDeinit(void *pvParameters);

/**
 * @brief 向关节电机发送控制信号
 *
 * @param pvParameters 任务参数，未使用
 * @note DM4310 采用一问一答机制，
 * 在发送一电机的控制信号后会触发中断处理接收到的反馈信息。 理论频率 500Hz。
 * (osPriorityAboveNormal)
 */
__weak void vTaskJointControl(void *pvParameters);

/**
 * @brief 传感器数据处理
 *
 * @param pvParameters 任务参数，未使用
 * @note 读取 BMI088 传感器数据，理论频率 1000Hz。 (osPriorityAboveNormal)
 */
__weak void vTaskSensor(void *pvParameters);

extern xTaskHandle xTaskLED_GHandle;

extern xTaskHandle wheelReceiveTaskHandle;
extern xTaskHandle wheelControlTaskHandle;

extern xTaskHandle readDbusTaskHandle;

extern xTaskHandle jointInitTaskHandle;
extern xTaskHandle jointDeinitTaskHandle;
extern xTaskHandle jointControlTaskHandle;

extern xTaskHandle sensorTaskHandle;

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

extern TickType_t tickCount;
extern bool jointInited;

extern SPI imuSPI;
extern BMI088 imu;
extern BMI088::Data_t *imuData;