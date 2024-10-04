#include "robotparticipant.h"

RobotParticipant::RobotParticipant(ParticipantSideEnum pS, QObject *parent)
    : Participant{ParticipantTypeEnum::PLAYER, pS, parent}
{}
