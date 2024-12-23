#include "WheelLeg/tasks.hpp"

void vTaskLED_G(void *pvParameters)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

    while (1) {
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
        osDelay(500);
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
        osDelay(500);
    }

    vTaskDelete(xTaskLED_GHandle);
}

xTaskHandle xTaskLED_GHandle;