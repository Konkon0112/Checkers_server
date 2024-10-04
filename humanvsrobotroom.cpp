#include "humanvsrobotroom.h"

HumanVsRobotRoom::HumanVsRobotRoom(QObject *parent)
    : RoomBase{RoomBase::RoomState::ACTIVE, parent}
// Active because it only needs one player who initiates the
// start then the robot player is automatically created
{
    roomType = RoomBase::RoomType::HUMAN_VS_ROBOT;
}
