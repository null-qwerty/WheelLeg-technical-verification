#pragma once

#include "main.h"

/**
 * @brief class Connectivity
 * 通信接口类，将通信所需的方法抽象出来，抽象类
 *
 * 接口定义：
 * Connectivity::init() 初始化通信口，纯虚函数
 * Connectivity::setTxHeader(void*) 设置发送帧头，纯虚函数
 * Connectivity::setReceiveBufferSize(uint16_t) 设置接收缓冲区大小
 * Connectivity::setSendBufferSize(uint16_t) 设置发送缓冲区大小

 * Connectivity::getRxHeader() 获取接收帧头部，纯虚函数
 * Connectivity::getReceiveBuffer() 获取接收缓冲区
 * Connectivity::getSendBuffer() 获取发送缓冲区
 * Connectivity::getReceiveBufferSize() 获取接收缓冲区大小
 * Connectivity::getSendBufferSize() 获取发送缓冲区大小
 *
 * Connectivity::sendMessage() 发送消息，纯虚函数
 * Connectivity::receiveMessage() 接收消息，纯虚函数
 */
class Connectivity {
public:
    Connectivity() = default;
    ~Connectivity() = default;

    /**
     * @brief 初始化
     *
     * @return Connectivity& 自身引用
     */
    virtual Connectivity &init() = 0;

    /**
     * @brief 设置发送帧头
     *
     * @param txHeader 设置的结构指针
     * @return Connectivity& 自身引用
     */
    virtual Connectivity &setTxHeader(void *txHeader) = 0;
    /**
     * @brief 设置接收缓冲区大小
     *
     * @param size 缓冲区大小
     * @return Connectivity& 自身引用
     */
    Connectivity &setReceiveBufferSize(uint16_t size);
    /**
     * @brief 设置发送缓冲区大小
     *
     * @param size 缓冲区大小
     * @return Connectivity& 自身引用
     */
    Connectivity &setSendBufferSize(uint16_t size);

    /**
     * @brief 获取接收帧头
     *
     * @return void* 结构指针
     */
    virtual void *getRxHeader() = 0;
    /**
     * @brief 获取接收缓冲区
     *
     * @return uint8_t* 缓冲区指针
     */
    uint8_t *getReceiveBuffer();
    /**
     * @brief 获取发生缓冲区
     *
     * @return uint8_t* 缓冲区指针
     */
    uint8_t *getSendBuffer();
    /**
     * @brief 接收缓冲区大小
     *
     * @return uint16_t 缓冲区大小
     */
    uint16_t getReceiveBufferSize();
    /**
     * @brief 获取发送缓冲区大小
     *
     * @return uint16_t 缓冲区大小
     */
    uint16_t getSendBufferSize();

    /**
     * @brief 发送消息
     *
     * @return uint8_t 状态 state
     */
    virtual uint8_t sendMessage() = 0;
    /**
     * @brief 接收消息
     *
     * @return uint8_t 状态 state
     */
    virtual uint8_t receiveMessage() = 0;

    Connectivity &operator=(const Connectivity &other);

protected:
    void *pxTxHeader;
    void *pxRxHeader;
    uint8_t *pReceiveBuffer, *pSendBuffer;
    uint16_t receiveBufferSize, sendBufferSize;
};