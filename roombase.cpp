#include "roombase.h"
#include "humanparticipant.h"

RoomBase::RoomBase(RoomState rs, QObject *parent)
    : QObject{parent}
{
    roomState = rs;
}

void RoomBase::join(QTcpSocket *socket)
{
    int pNum = countPlayersInRoom();
    HumanParticipant* hP;
    Participant::ParticipantTypeEnum pT = pNum >= 2? Participant::ParticipantTypeEnum::SPECTATOR:
                                              Participant::ParticipantTypeEnum::PLAYER;
    hP = new HumanParticipant(socket,
                              pT,
                              Participant::ParticipantSideEnum::NONE,
                              this);
    this->pList.append(hP);
    //TODO: connect signals between room and participant
    // - start game
    // - initiate step
    // - step happened
    // - send steps so far
    // - initiate undo
    // - approve undo
    // - game over

    if(pNum == 1){ // That means now there are two players now
        //TODO: emit start game signal
        //
    }
}

RoomBase::RoomState RoomBase::getRoomState() const
{
    return roomState;
}

int RoomBase::countPlayersInRoom()
{
    int numofP = 0;
    for(int i = 0; i < pList.size(); i++){
        if(pList.at(i)->isPlayerType(Participant::ParticipantTypeEnum::PLAYER)){
            numofP++;
        }
    }
    return numofP;
}

void RoomBase::readyRead()
{

}
