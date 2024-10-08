#include <QRandomGenerator>

#include "roombase.h"
#include "humanparticipant.h"

RoomBase::RoomBase(RoomState rs, QObject *parent)
    : QObject{parent}
{
    roomState = rs;
    gameModel = new GameModel(this);
    connect(gameModel, SIGNAL(stepHappenedSignal(QString)), this, SLOT(stepHappenedSlot(QString)));
}

void RoomBase::join(QTcpSocket *socket)
{
    int pNum = pList.length();
    HumanParticipant* hP;

    Participant::ParticipantTypeEnum pT = pNum >= 2? Participant::ParticipantTypeEnum::SPECTATOR:
                                              Participant::ParticipantTypeEnum::PLAYER;
    Participant::ParticipantSideEnum pS;
    if(pNum == 0){
        int randomNumber = QRandomGenerator::global()->bounded(1, 101);

        pS =
            randomNumber > 50? Participant::ParticipantSideEnum::DARK:
                Participant::ParticipantSideEnum::LIGHT;
    } else if(pNum == 1){
        pS =
            pList.at(0)->isPlayerSide(Participant::ParticipantSideEnum::DARK)?
                Participant::ParticipantSideEnum::LIGHT:
                Participant::ParticipantSideEnum::DARK;
    } else {
        pS = Participant::ParticipantSideEnum::NONE;
    }

    hP = new HumanParticipant(socket,
                              pT,
                              pS,
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
    // - player quit
    connect(hP, SIGNAL(playerQuitSignal()), this, SLOT(playerQuitSlot()));


    this->pList.append(hP);

    if(pNum == 1){ // That means now there are two players now
        emit gameStarted();
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
    emit gameStarted();
}

void RoomBase::stepInitiatedSlot(QString step)
{
    // Check if right player
    //  - player on turn
    //  - player color and piece color match
    QObject* signalSender = sender();
    Participant* participant = qobject_cast<Participant*>(signalSender);
    if(!participant->isPlayerSide(gameModel->getColorOnTurn())) return;

    gameModel->passStepForward(step);
}

void RoomBase::playerQuitSlot()
{
    QObject* signalSender = sender();
    HumanParticipant* participant = qobject_cast<HumanParticipant*>(signalSender);

    emit playerQuitGameSignal(participant->getSocket());

    pList.removeAll(participant);
    qInfo() << participant << "left the game";

    delete participant;

}

void RoomBase::undoInitiatedSlot()
{

}

void RoomBase::approveUndoSlot()
{

}

void RoomBase::stepHappenedSlot(QString step)
{
    emit stepHappenedSignal(step);
}

