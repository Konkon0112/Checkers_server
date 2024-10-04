#include "humanparticipant.h"

HumanParticipant::HumanParticipant(QTcpSocket* socket,
                                   ParticipantTypeEnum pT,
                                   ParticipantSideEnum pS,
                                   QObject *parent)
    : Participant{socket, pT, pS, parent}
{}
