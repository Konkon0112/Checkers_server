#include <QRandomGenerator>

#include "roombase.h"
#include "humanparticipant.h"

RoomBase::RoomBase(RoomState rs, QObject *parent)
    : QObject{parent}
{
    roomState = rs;
    gameModel = new GameModel(this);
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

    //TODO: connect signals between room and participant
    // - start game
    connect(this, SIGNAL(gameStarted()), hP, SLOT(gameStartedSlot()));
    // - initiate step
    connect(hP, SIGNAL(stepInitiatedSignal(QString)), this, SLOT(stepInitiatedSlot(QString)));
    // - step happened
    connect(this, SIGNAL(stepHappenedSignal(QString)), hP, SLOT(stepHappenedSlot(QString)));
    // - send steps so far (maybe don't need this)

    // - turn changed
    connect(this, SIGNAL(turnChangedSignal(Participant::ParticipantSideEnum)), hP, SLOT(turnChangedSignal(Participant::ParticipantSideEnum)));
    // - initiate undo
    connect(hP, SIGNAL(undoInitiatedSignal()), this, SLOT(undoInitiatedSlot()));
    // - approve undo
    connect(hP, SIGNAL(approveUndoSignal()), this, SLOT(approveUndoSlot()));
    // - undo approved
    connect(this, SIGNAL(undoApprovedSignal()), hP, SLOT(undoApprovedSlot()));
    // - game over
    connect(this, SIGNAL(gameOver(Participant::ParticipantSideEnum)), hP, SLOT(gameOverSlot(Participant::ParticipantSideEnum)));

    this->pList.append(hP);

    if(pNum == 1){ // That means now there are two players now
        startGame();
    }
}

RoomBase::RoomState RoomBase::getRoomState() const
{
    return roomState;
}

RoomBase::RoomType RoomBase::getRoomType() const
{
    return roomType;
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

void RoomBase::startGame()
{
    QList<Participant*> players;
    for(int i = 0; i < pList.length(); i++){
        if(pList.at(i)->isPlayerType(Participant::ParticipantTypeEnum::PLAYER))
            players.append(pList.at(i));
    }

    if(players.at(0)->isPlayerSide(Participant::ParticipantSideEnum::NONE)&&
       players.at(1)->isPlayerSide(Participant::ParticipantSideEnum::NONE)){ // Player vs player case

        int randomNumber = QRandomGenerator::global()->bounded(1, 101);

        Participant::ParticipantSideEnum fPlayerSide =
            randomNumber > 50? Participant::ParticipantSideEnum::DARK:
                                Participant::ParticipantSideEnum::LIGHT;

        Participant::ParticipantSideEnum sPlayerSide =
            fPlayerSide == Participant::ParticipantSideEnum::DARK?
                Participant::ParticipantSideEnum::LIGHT:
                Participant::ParticipantSideEnum::DARK;

        players.at(0)->setPSide(fPlayerSide);
        players.at(1)->setPSide(sPlayerSide);
    } else {
        Participant::ParticipantSideEnum fPlayerSide = players.at(0)->getPSide();
        Participant::ParticipantSideEnum sPlayerSide = players.at(1)->getPSide();

        if(fPlayerSide == Participant::ParticipantSideEnum::NONE){
            Participant::ParticipantSideEnum newFPlayerSide =
                sPlayerSide == Participant::ParticipantSideEnum::DARK?
                    Participant::ParticipantSideEnum::LIGHT:
                    Participant::ParticipantSideEnum::DARK;

            players.at(0)->setPSide(newFPlayerSide);

        } else if(sPlayerSide == Participant::ParticipantSideEnum::NONE) {

            Participant::ParticipantSideEnum newSPlayerSide =
                fPlayerSide == Participant::ParticipantSideEnum::DARK?
                    Participant::ParticipantSideEnum::LIGHT:
                    Participant::ParticipantSideEnum::DARK;

            players.at(0)->setPSide(newSPlayerSide);
        } else {
            qDebug() << "Something went wrong while adding players to room";
        }
    }
    emit gameStarted();
}

void RoomBase::stepInitiatedSlot(QString step)
{
    // Check if right player
    //  - player on turn
    //  - player color and piece color match
}

void RoomBase::playerQuitSlot()
{

}

void RoomBase::undoInitiatedSlot()
{

}

void RoomBase::approveUndoSlot()
{

}

