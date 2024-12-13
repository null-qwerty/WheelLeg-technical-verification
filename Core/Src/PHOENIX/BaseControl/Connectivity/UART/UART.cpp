#include "PHOENIX/BaseControl/Connectivity/UART/UART.hpp"
#include "stm32f4xx_hal_uart.h"

UART::UART(UART_HandleTypeDef *huart)
    : huart(huart)
{
}

UART::~UART(void)
{
}

uint8_t UART::sendMessage()
{
    return HAL_UART_Transmit(huart, pSendBuffer, sendBufferSize, 0xf);
}

uint8_t UART::receiveMessage()
{
    return HAL_UART_Receive(huart, pReceiveBuffer, receiveBufferSize, 0xf);
}

UART &UART::init()
{
    HAL_UART_DeInit(huart);
    HAL_UART_Init(huart);
    return *this;
}