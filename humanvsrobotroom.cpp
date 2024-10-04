#include "humanvsrobotroom.h"
#include "robotparticipant.h"

HumanVsRobotRoom::HumanVsRobotRoom(Participant::ParticipantSideEnum robotS, QObject *parent)
    : RoomBase{RoomBase::RoomState::ACTIVE, parent}
// Active because it only needs one player who initiates the
// start then the robot player is automatically created
{
    roomType = RoomBase::RoomType::HUMAN_VS_ROBOT;
    RobotParticipant* rParticipant = new RobotParticipant(robotS, this);
    pList.append(rParticipant);
}
