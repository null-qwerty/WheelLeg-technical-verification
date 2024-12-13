#pragma once

#include "can.h"
#include "stm32f4xx_hal_can.h"

#include "PHOENIX/BaseControl/Connectivity/Connectivity.hpp"

/**
 * @brief CAN 通信类，封装了 HAL 库的 CAN 相关函数
 *
 * @note 重载了赋值运算符为浅拷贝
 */
class CAN : public Connectivity {
public:
    CAN(CAN_HandleTypeDef *hcan, CAN_FilterTypeDef &filter);
    ~CAN();

    CAN &init() override;
    CAN &setTxHeader(void *txHeader) override;
    void *getRxHeader() override;
    uint8_t sendMessage() override;
    uint8_t receiveMessage() override;
    CAN_FilterTypeDef &getFilter();
    /**
     * @brief 重载赋值运算符为浅拷贝
     *
     * @param other 赋值对象
     * @return CAN& 返回自身引用
     */
    CAN &operator=(const CAN &other);

private:
    CAN_HandleTypeDef *hcan = nullptr;
    CAN_TxHeaderTypeDef can_tx = {};
    CAN_RxHeaderTypeDef can_rx = {};
    uint32_t can_tx_mailbox = 0;

    CAN_FilterTypeDef filter = {};

    uint8_t dataSendBuffer[8] = {};
    uint8_t dataReceiveBuffer[8] = {};
};