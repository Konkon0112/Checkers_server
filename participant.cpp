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

bool Participant::isPlayerSide(ParticipantSideEnum pC)
{
    return pC == pSide;
}

Participant::ParticipantTypeEnum Participant::getPType() const
{
    return pType;
}

void Participant::setPSide(ParticipantSideEnum newPSide)
{
    pSide = newPSide;
}

Participant::ParticipantSideEnum Participant::getPSide() const
{
    return pSide;
}
