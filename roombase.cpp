#include <QRandomGenerator>

#include "roombase.h"
#include "humanparticipant.h"
#include "customexception.h"

RoomBase::RoomBase(RoomState rs, QObject *parent)
    : QObject{parent}
{
    roomState = rs;
    gameModel = new GameModel(this);
    connect(gameModel, SIGNAL(stepHappenedSignal(QString,Participant::ParticipantSideEnum)),
            this, SLOT(stepHappenedSlot(QString,Participant::ParticipantSideEnum)));

    connect(gameModel, SIGNAL(undoHappenedSignal(QString,Participant::ParticipantSideEnum)),
            this, SLOT(undoHappenedSlot(QString,Participant::ParticipantSideEnum)));

    connect(gameModel, SIGNAL(gameOverSignal(Participant::ParticipantSideEnum)),
            this, SLOT(gameOverSlot(Participant::ParticipantSideEnum)));
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

    // - start game
    connect(this, SIGNAL(gameStartedSignal(Participant::ParticipantSideEnum,QString)),
            hP,SLOT(gameStartedSlot(Participant::ParticipantSideEnum,QString)));
    // - initiate step
    connect(hP, SIGNAL(stepInitiatedSignal(QString)), this, SLOT(stepInitiatedSlot(QString)));
    // - step happened
    connect(this, SIGNAL(stepHappenedSignal(QString,Participant::ParticipantSideEnum)),
            hP, SLOT(stepHappenedSlot(QString,Participant::ParticipantSideEnum)));
    // - initiate undo
    connect(hP, SIGNAL(undoInitiatedSignal()), this, SLOT(undoInitiatedSlot()));
    // - undo needs approval
    connect(this, SIGNAL(undoNeedsApproval(Participant::ParticipantSideEnum)), hP, SLOT(undoNeedsApprovalSlot(Participant::ParticipantSideEnum)));
    // - approve undo
    connect(hP, SIGNAL(approveUndoSignal()), this, SLOT(approveUndoSlot()));
    // - reject undo
    connect(hP, SIGNAL(rejectUndoSignal()), this, SLOT(rejectUndoSlot()));
    // - undo happened
    connect(this, SIGNAL(undoHappenedSignal(QString,Participant::ParticipantSideEnum)),
            hP, SLOT(undoHappenedSlot(QString,Participant::ParticipantSideEnum)));
    // - game over
    connect(this, SIGNAL(gameOverSignal(Participant::ParticipantSideEnum)), hP, SLOT(gameOverSlot(Participant::ParticipantSideEnum)));
    // - player quit
    connect(hP, SIGNAL(playerQuitSignal()), this, SLOT(playerQuitSlot()));


    this->pList.prepend(hP);

    if(pNum == 1){
        roomState = RoomState::ACTIVE;
        gameModel->startGame();
    }

    if(pNum >= 1){
        emit gameStartedSignal(gameModel->getColorOnTurn(), gameModel->getJoinedStepStr());
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

void RoomBase::dealWithDisconnectedParticipant(QTcpSocket* socket)
{
    for(int i = 0; i < pList.length(); i++){

        if(pList.at(i)->usingThisSocket(socket)){
            HumanParticipant* hP = qobject_cast<HumanParticipant*>(pList.at(i));
            int pNum = countPlayersInRoom();

            if(roomState == RoomBase::RoomState::ACTIVE &&
                hP->getPType() == Participant::ParticipantTypeEnum::PLAYER && pNum == 2){
                // If one of the player disconnects than the other one will be the winner.
                Participant::ParticipantSideEnum winner =
                    hP->getPSide() == Participant::ParticipantSideEnum::DARK?
                        Participant::ParticipantSideEnum::LIGHT : Participant::ParticipantSideEnum::DARK;
                emit gameOverSignal(winner);
                roomState == RoomBase::RoomState::FINISHED;
            }

            pList.removeAll(hP);
            qInfo() << hP  << "left the game";
            delete hP;

            if(pList.length() == 0) emit removeRoomFromListSignal();
        }
    }
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

void RoomBase::stepInitiatedSlot(QString step)
{
    QObject* signalSender = sender();
    Participant* participant = qobject_cast<Participant*>(signalSender);
    if(undoInitiatedBy != Participant::ParticipantSideEnum::NONE) {
        participant->receiveNotification(ToastTypeEnum::WARNING, "There is an active undo request.");
    }
    // Check if right player
    //  - player on turn
    if(participant->isPlayerSide(Participant::ParticipantSideEnum::NONE)){
        participant->receiveNotification(ToastTypeEnum::WARNING, "Spectators cannot initiate moves!");
        return;
    }
    if(!participant->isPlayerSide(gameModel->getColorOnTurn())){
        participant->receiveNotification(ToastTypeEnum::WARNING, "It is not your turn to move yet!");
        return;
    }

    try {
        gameModel->executeStep(step);
    } catch(CustomException* e){
        participant->receiveNotification(e->getToastType(), e->getMessage());
    }
}

void RoomBase::undoInitiatedSlot()
{
    QObject* signalSender = sender();
    Participant* participant = qobject_cast<Participant*>(signalSender);

    if(undoInitiatedBy != Participant::ParticipantSideEnum::NONE){
        participant->receiveNotification(ToastTypeEnum::WARNING, "There is an active undo request.");
        return;
    }

    Participant::ParticipantSideEnum pS = participant->getPSide();

    Participant::ParticipantSideEnum approvingSide =
        pS == Participant::ParticipantSideEnum::DARK?
            Participant::ParticipantSideEnum::LIGHT : Participant::ParticipantSideEnum::DARK;

    undoInitiatedBy = pS;
    emit undoNeedsApproval(approvingSide);
}

void RoomBase::undoHappenedSlot(QString newStepsSoFar, Participant::ParticipantSideEnum nextC)
{
    emit undoHappenedSignal(newStepsSoFar, nextC);
}

void RoomBase::approveUndoSlot()
{
    QObject* signalSender = sender();
    Participant* participant = qobject_cast<Participant*>(signalSender);

    Participant::ParticipantSideEnum approvingSide =
        undoInitiatedBy == Participant::ParticipantSideEnum::DARK?
            Participant::ParticipantSideEnum::LIGHT : Participant::ParticipantSideEnum::DARK;

    if(participant->getPSide() != approvingSide){
        participant->receiveNotification(ToastTypeEnum::WARNING, "You cannot approve this undo request!");
        return;
    }
    gameModel->undoStep(undoInitiatedBy);

    emit undoApprovedSignal();
    undoInitiatedBy = Participant::ParticipantSideEnum::NONE;
}

void RoomBase::rejectUndoSlot()
{
    QObject* signalSender = sender();
    Participant* participant = qobject_cast<Participant*>(signalSender);

    Participant::ParticipantSideEnum approvingSide =
        undoInitiatedBy == Participant::ParticipantSideEnum::DARK?
            Participant::ParticipantSideEnum::LIGHT : Participant::ParticipantSideEnum::DARK;

    if(participant->getPSide() != approvingSide){
        participant->receiveNotification(ToastTypeEnum::WARNING, "You cannot reject this undo request!");
        return;
    }

    for(int i = 0; i < pList.length(); i++){
        if(pList[i]->isPlayerSide(undoInitiatedBy)){
            pList[i]->receiveNotification(ToastTypeEnum::INFO, "Undo request rejected!");
            break;
        }
    }

    undoInitiatedBy = Participant::ParticipantSideEnum::NONE;
}

void RoomBase::stepHappenedSlot(QString step, Participant::ParticipantSideEnum newTurnColor)
{
    emit stepHappenedSignal(step, newTurnColor);
}

void RoomBase::gameOverSlot(Participant::ParticipantSideEnum winner)
{
    emit gameOverSignal(winner);
}

