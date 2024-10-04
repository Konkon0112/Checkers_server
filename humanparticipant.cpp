#include "humanparticipant.h"

HumanParticipant::HumanParticipant(QTcpSocket* s,
                                   ParticipantTypeEnum pT,
                                   ParticipantSideEnum pS,
                                   QObject *parent)
    : Participant{pT, pS, parent}
{
    socket = s;
}
