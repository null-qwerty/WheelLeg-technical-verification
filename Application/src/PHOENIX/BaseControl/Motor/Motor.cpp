#include "PHOENIX/BaseControl/Motor/Motor.hpp"

Motor::Motor(Connectivity &connectivity, uint16_t send_id, uint16_t receive_id)
    : send_id(send_id)
    , receive_id(receive_id)
    , connectivity(connectivity)
{
}

bool Motor::ifInitialed()
{
    return initialed;
}

Motor::MotorState &Motor::getState(void)
{
    return state;
}

Motor::MotorState &Motor::getTargetState(void)
{
    return targetState;
}

pidController &Motor::getAngleLoopController()
{
    return this->angleLoop;
}
pidController &Motor::getSpeedLoopController()
{
    return this->speedLoop;
}
pidController &Motor::getCurrentLoopController()
{
    return this->currentLoop;
}

uint16_t Motor::getSendId()
{
    return this->send_id;
}

uint16_t Motor::getReceiveId()
{
    return this->receive_id;
}