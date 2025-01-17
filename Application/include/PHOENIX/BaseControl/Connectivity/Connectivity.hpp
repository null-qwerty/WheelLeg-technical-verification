#pragma once

#include "main.h"

/**
 * @brief class Connectivity
 * 通信接口类，将通信所需的方法抽象出来，抽象类
 *
 * 接口定义：
 * Connectivity::init() 初始化通信口，纯虚函数

 * Connectivity::getReceiveFrame() 获取接收帧缓冲区，纯虚函数
 * Connectivity::getSendFrame() 获取发送帧缓冲区，纯虚函数
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
     * @brief 获取接收帧缓冲区
     *
     * @return void* 结构指针
     */
    virtual void *getReceiveFrame() = 0;
    /**
     * @brief 获取发送帧缓冲区
     *
     * @return void* 结构指针
     */
    virtual void *getSendFrame() = 0;
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
};