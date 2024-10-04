#include "participant.h"
#include "packettypekeeperservice.h"

Participant::Participant(ParticipantTypeEnum pT,
                         ParticipantSideEnum pS,
                         QObject *parent)
    : QObject{parent}
{
    pType = pT;
    pSide = pS;
    ptKeeper = new PacketTypeKeeperService(this);
}

bool Participant::isPlayerType(ParticipantTypeEnum pT)
{
    return pT == pType;
}
