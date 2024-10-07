#include "robotparticipant.h"

RobotParticipant::RobotParticipant(ParticipantSideEnum pS, QObject *parent)
    : Participant{ParticipantTypeEnum::PLAYER, pS, parent}
{}

void RobotParticipant::gameStartedSlot()
{

}

void RobotParticipant::stepHappenedSlot(QString step)
{

}

void RobotParticipant::turnChangedSignal(ParticipantSideEnum nextOnTurn)
{

}

void RobotParticipant::undoApprovedSlot()
{

}

void RobotParticipant::turnChangedSlot()
{

}
