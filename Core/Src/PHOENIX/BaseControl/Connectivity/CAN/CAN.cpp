#include "PHOENIX/BaseControl/Connectivity/CAN/CAN.hpp"
#include "PHOENIX/BaseControl/Connectivity/Connectivity.hpp"

#include "stm32f4xx_hal_can.h"
#include "stm32f4xx_hal_def.h"

CAN::CAN(CAN_HandleTypeDef *hcan, CAN_FilterTypeDef &filter)
    : Connectivity()
    , hcan(hcan)
    , filter(filter)
{
    this->pxRxHeader = &can_rx;
    this->pxTxHeader = &can_tx;
    this->pReceiveBuffer = dataReceiveBuffer;
    this->pSendBuffer = dataSendBuffer;
}

CAN::~CAN()
{
}

CAN &CAN::init()
{
    while (HAL_CAN_ConfigFilter(hcan, &filter) != HAL_OK)
        ;
    while (HAL_CAN_Start(hcan) != HAL_OK)
        ;
    while (HAL_CAN_ActivateNotification(
               hcan, filter.FilterFIFOAssignment == CAN_RX_FIFO0 ?
                         CAN_IT_RX_FIFO0_MSG_PENDING :
                         CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
        ;

    return *this;
}

CAN &CAN::setTxHeader(void *txHeader)
{
    can_tx = *((CAN_TxHeaderTypeDef *)txHeader);
    return *this;
}

void *CAN::getRxHeader(void)
{
    return &can_rx;
}

CAN_FilterTypeDef &CAN::getFilter()
{
    return this->filter;
}

uint8_t CAN::sendMessage(void)
{
    return HAL_CAN_AddTxMessage(hcan, &can_tx, dataSendBuffer, &can_tx_mailbox);
}

uint8_t CAN::receiveMessage(void)
{
    return HAL_CAN_GetRxMessage(hcan, filter.FilterFIFOAssignment, &can_rx,
                                dataReceiveBuffer);
}

CAN &CAN::operator=(const CAN &other)
{
    hcan = other.hcan;
    can_tx = other.can_tx;
    can_rx = other.can_rx;
    can_tx_mailbox = other.can_tx_mailbox;
    filter = other.filter;

    return *this;
}