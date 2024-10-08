#include "robotparticipant.h"

RobotParticipant::RobotParticipant(ParticipantSideEnum pS, QObject *parent)
    : Participant{ParticipantTypeEnum::PLAYER, pS, parent}
{}

bool RobotParticipant::usingThisSocket(QTcpSocket *soc)
{
    return false;
}

void RobotParticipant::gameStartedSlot(Participant::ParticipantSideEnum nextOnTurn, QString stepsSoFar)
{

}

void RobotParticipant::stepHappenedSlot(QString step)
{

}

void RobotParticipant::undoApprovedSlot()
{

}

void RobotParticipant::turnChangedSlot(Participant::ParticipantSideEnum nextOnTurn)
{

}
