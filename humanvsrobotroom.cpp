#include "humanvsrobotroom.h"
#include "robotparticipant.h"
#include "humanparticipant.h"

HumanVsRobotRoom::HumanVsRobotRoom(Participant::ParticipantSideEnum robotS, QObject *parent)
    : RoomBase{RoomBase::RoomState::ACTIVE, parent}
// Active because it only needs one player who initiates the
// start then the robot player is automatically created
{
    roomType = RoomBase::RoomType::HUMAN_VS_ROBOT;
    RobotParticipant* rParticipant = new RobotParticipant(robotS, this);

    // - start game
    connect(this, SIGNAL(gameStartedSignal(Participant::ParticipantSideEnum, QString)), rParticipant, SLOT(gameStartedSlot(Participant::ParticipantSideEnum, QString)));
    // - initiate step
    connect(rParticipant, SIGNAL(stepInitiatedSignal(QString)), this, SLOT(stepInitiatedSlot(QString)));
    // - step happened
    connect(this, SIGNAL(stepHappenedSignal(QString)), rParticipant, SLOT(stepHappenedSlot(QString)));
    // - turn changed
    connect(this, SIGNAL(turnChangedSignal(Participant::ParticipantSideEnum)), rParticipant, SLOT(turnChangedSlot(Participant::ParticipantSideEnum)));
    // - undo needs approval
    connect(this, SIGNAL(undoNeedsApproval(Participant::ParticipantSideEnum)), rParticipant, SLOT(undoNeedsApprovalSlot(Participant::ParticipantSideEnum)));
    // - approve undo
    connect(rParticipant, SIGNAL(approveUndoSignal()), this, SLOT(approveUndoSlot()));
    // - undo happened
    connect(this, SIGNAL(undoHappenedSignal(QString)), rParticipant, SLOT(undoHappenedSlot(QString)));

    pList.append(rParticipant);
}

void HumanVsRobotRoom::setUpContinuedGame(Participant::ParticipantSideEnum playerOnTurn, QString stepStr)
{
    // recreate table in game model
    playerOnTurn = playerOnTurn;
    gameModel->setColorOnTurn(playerOnTurn);
    QStringList steps = stepStr.split(';');
    for(int i = 0; i < steps.length(); i++){
        gameModel->passStepForward(steps.at(i));
    }
}

void HumanVsRobotRoom::playerQuitSlot()
{
    // This can only be received from human player here
    QObject* signalSender = sender();
    if(!signalSender) return;
    HumanParticipant* participant = qobject_cast<HumanParticipant*>(signalSender);

    disconnect(this, nullptr, participant, nullptr);
    disconnect(participant, nullptr, this, nullptr);

    emit playerQuitGameSignal(participant->getSocket());

    pList.removeAll(participant);
    qInfo() << participant << "left the game";

    delete participant;

    roomState = RoomBase::RoomState::FINISHED;
    emit removeRoomFromListSignal(); // Only player left is a robot
}
