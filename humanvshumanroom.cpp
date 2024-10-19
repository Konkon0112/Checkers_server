#include "humanvshumanroom.h"
#include "humanparticipant.h"

HumanVsHumanRoom::HumanVsHumanRoom(QObject *parent)
    : RoomBase{RoomBase::RoomState::WAITING_FOR_PLAYERS, parent}
{
    roomType = RoomBase::RoomType::HUMAN_VS_HUMAN;
}

void HumanVsHumanRoom::playerQuitSlot()
{
    QObject* signalSender = sender();
    if(!signalSender) return;
    HumanParticipant* participant = qobject_cast<HumanParticipant*>(signalSender);

    int pNum = countPlayersInRoom();
    disconnect(this, nullptr, participant, nullptr);
    disconnect(participant, nullptr, this, nullptr);


    if( roomState == RoomState::ACTIVE &&
        participant->getPType() == Participant::ParticipantTypeEnum::PLAYER &&
        pNum == 2){
        Participant::ParticipantSideEnum winner =
            participant->getPSide() == Participant::ParticipantSideEnum::DARK?
                Participant::ParticipantSideEnum::LIGHT : Participant::ParticipantSideEnum::DARK;
        emit gameOverSignal(winner);
        roomState == RoomBase::RoomState::FINISHED;
    }

    emit playerQuitGameSignal(participant->getSocket());

    pList.removeAll(participant);
    qInfo() << participant << "left the game";

    delete participant;

    if(pNum == 1 && roomState == RoomState::ACTIVE) emit removeRoomFromListSignal();
}
