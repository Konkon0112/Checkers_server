#include "participant.h"

Participant::Participant(QTcpSocket* s,
                         ParticipantTypeEnum pT,
                         ParticipantSideEnum pS,
                         QObject *parent)
    : QObject{parent}
{
    socket = s;
    pType = pT;
    pSide = pS;
}


