#include "BaseControl/Controller/pidController.hpp"
#include "Math/Trigonometric.hpp"
#include "tasks.hpp"

#include "Filter/Mahony.hpp"

#include "tim.h"

float yaw, pitch, roll;
Mahony mahony(1000.0f);

void vTaskSensor(void *pvParameters)
{
    Quaternion q = Quaternion::identity();
    Vector3f gyro = Vector3f::Zero();
    Vector3f accel = Vector3f::Zero();

    imu.init();
    mahony.init(1.f);
    imuData = (BMI088::Data_t *)imu.getData();

    TickType_t xLastWakeTime;
    TickType_t xFrequency = pdMS_TO_TICKS(1);
    xLastWakeTime = xTaskGetTickCount();
    while (1) {
        imu.getData();
        gyro[0] = imuData->gyro.roll;
        gyro[1] = imuData->gyro.pitch;
        gyro[2] = imuData->gyro.yaw;
        accel[0] = imuData->accel.x;
        accel[1] = imuData->accel.y;
        accel[2] = imuData->accel.z;

        q = mahony.update(gyro, accel);
        auto euler = q.toEulerAngles();
        yaw = RAND_TO_DEGREE(euler[2]);
        pitch = RAND_TO_DEGREE(euler[1]);
        roll = RAND_TO_DEGREE(euler[0]);

        xTaskNotifyGive(temperatureHoldTaskHandle);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

float pwm;

void vTaskTemperatureHold(void *pvParameters)
{
    HAL_TIM_Base_Start(&htim10);
    HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);
    pidController temperatureHold(1800, 0.01, 0., 4500, -4500);
    float target = 40.0f;
    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        pwm = temperatureHold.calculate(target, imuData->temperature);
        if (pwm <= 0)
            pwm = 0;
        __HAL_TIM_SetCompare(&htim10, TIM_CHANNEL_1, pwm);
        mahony.yawZeroDriftOffset(DEGREE_TO_RAND(1.13333e-4));
    }
}

xTaskHandle sensorTaskHandle;
xTaskHandle temperatureHoldTaskHandle;

SPI imuSPI = SPI(&hspi1, SPI::dmaOption::RX_TX);
BMI088 imu(imuSPI);
BMI088::Data_t *imuData;