#include "robotparticipant.h"

RobotParticipant::RobotParticipant(ParticipantSideEnum pS, QObject *parent)
    : Participant{ParticipantTypeEnum::PLAYER, pS, parent}
{}

void RobotParticipant::gameStartedSlot()
{

}

void RobotParticipant::stepHappenedSlot()
{

}

void RobotParticipant::gameOverSlot()
{

}

void RobotParticipant::turnChangedSlot()
{

}
