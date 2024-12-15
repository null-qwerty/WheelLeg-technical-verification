#include "Utils/Utils.hpp"
#include "stm32_hal_legacy.h"
#include "stm32f4xx_hal_tim.h"
#include "tim.h"

void delay_us(uint32_t us)
{
    __HAL_TIM_SetCounter(&htim14, 0);
    HAL_TIM_Base_Start(&htim14);
    while (__HAL_TIM_GetCounter(&htim14) < us)
        ;
    HAL_TIM_Base_Stop(&htim14);
}
