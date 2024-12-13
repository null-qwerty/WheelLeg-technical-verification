#include "PHOENIX/BaseControl/Connectivity/Connectivity.hpp"

Connectivity &Connectivity::operator=(const Connectivity &other)
{
    this->pxRxHeader = other.pxRxHeader;
    this->pxTxHeader = other.pxTxHeader;
    this->pReceiveBuffer = other.pReceiveBuffer;
    this->pSendBuffer = other.pSendBuffer;
    return *this;
}

Connectivity &Connectivity::setReceiveBufferSize(uint16_t size)
{
    this->receiveBufferSize = size;
    return *this;
}
Connectivity &Connectivity::setSendBufferSize(uint16_t size)
{
    this->sendBufferSize = size;
    return *this;
}

uint8_t *Connectivity::getReceiveBuffer()
{
    return pReceiveBuffer;
}
uint8_t *Connectivity::getSendBuffer()
{
    return pSendBuffer;
}

uint16_t Connectivity::getReceiveBufferSize()
{
    return receiveBufferSize;
}
uint16_t Connectivity::getSendBufferSize()
{
    return sendBufferSize;
}
