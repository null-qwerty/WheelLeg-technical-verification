#pragma once

#include "Motor.hpp"
#include "PHOENIX/BaseControl/Connectivity/CAN/CAN.hpp"

class DM4310 : public Motor {
public:
    DM4310(CAN &can, uint16_t send_id, uint16_t receive_id, int8_t cw);
    ~DM4310();

    DM4310 &init() final;

    DM4310 &encodeControlMessage() final;
    DM4310 &decodeFeedbackMessage() final;

private:
    uint8_t ifEnable = 0;

    const float DM4310_MAX_TAU = 10.f;
    const float DM4310_MAX_VEL = 30.f;
    const float PI = 3.141593f;

    uint16_t linearFloat2Uint(float x, float x_max, float x_min, uint8_t bits);
    float linearUint2Float(uint16_t x, float x_max, float x_min, uint8_t bits);

    float calculateControlData() final;
};
