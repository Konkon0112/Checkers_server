#include "robotparticipant.h"

RobotParticipant::RobotParticipant(QTcpSocket* socket, ParticipantSideEnum pS, QObject *parent)
    : Participant{socket, ParticipantTypeEnum::PLAYER, pS, parent}
{}
