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

void RobotParticipant::undoNeedsApprovalSlot(Participant::ParticipantSideEnum approvingSideColor)
{
    if(pSide != approvingSideColor) return;
    emit approveUndoSignal();
}

void RobotParticipant::undoHappenedSlot(QString newStepsSoFar)
{

}

void RobotParticipant::turnChangedSlot(Participant::ParticipantSideEnum nextOnTurn)
{

}
