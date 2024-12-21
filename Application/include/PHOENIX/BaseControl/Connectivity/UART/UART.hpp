#pragma once

#include "main.h"

#include "PHOENIX/BaseControl/Connectivity/Connectivity.hpp"

class UART : public Connectivity {
public:
    /**
     * @brief UART 构造函数
     *
     * @param huart UART 句柄
     */
    UART(UART_HandleTypeDef *huart);
    /**
     * @brief UART 析构函数
     *
     */
    ~UART(void);

    /**
     * @brief 发送数据
     *
     * @return status
     */
    uint8_t sendMessage() override;

    /**
     * @brief 接收数据
     *
     * @return status
     */
    uint8_t receiveMessage() override;

    UART &init() override;

    UART &setTxHeader(void *txHeader) override
    {
        pxTxHeader = txHeader;
        return *this;
    };
    void *getRxHeader() override
    {
        return nullptr;
    };

private:
    UART_HandleTypeDef *huart = nullptr;
};