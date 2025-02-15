#include "tasks.hpp"

#include "Filter/Mahony.hpp"

#include "BaseControl/Sensor/IST8310/IST8310.hpp"
#include "BaseControl/Connectivity/I2C/I2C.hpp"

float yaw, pitch, roll;
Mahony6Axis mahony(1000.0f);

I2C megI2C = I2C(&hi2c3);
IST8310 meg(megI2C, 0x0E);
IST8310::Data_t *megData;

void vTaskSensor(void *pvParameters)
{
    Quaternion q = Quaternion::identity();
    Vector3f gyro = Vector3f::Zero();
    Vector3f accel = Vector3f::Zero();

    imu.init();
    mahony.init(0.707f);
    imuData = (BMI088::Data_t *)imu.getData();

    meg.init();
    megData = (IST8310::Data_t *)meg.getData();

    TickType_t xLastWakeTime;
    TickType_t xFrequency = pdMS_TO_TICKS(1);
    xLastWakeTime = xTaskGetTickCount();
    while (1) {
        imu.getData();
        meg.getData();
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

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

xTaskHandle sensorTaskHandle;

SPI imuSPI = SPI(&hspi1, SPI::dmaOption::RX_TX);
BMI088 imu(imuSPI);
BMI088::Data_t *imuData;