#include <QStringList>
#include <QSet>
#include <QRandomGenerator>

#include "robotparticipant.h"
#include "validatordame.h"
#include "validatorpawn.h"
#include "robotstepcalculator.h"

RobotParticipant::RobotParticipant(ParticipantSideEnum pS, QObject *parent)
    : Participant{ParticipantTypeEnum::PLAYER, pS, parent}
{
    board = new Board(this);
    validators.append(new ValidatorPawn(this));
    validators.append(new ValidatorDame(this));
    pool = QThreadPool::globalInstance();
}

bool RobotParticipant::usingThisSocket(QTcpSocket *soc)
{
    return false;
}

void RobotParticipant::receiveNotification(ToastTypeEnum tt, QString msg)
{
    qDebug() << "Robot received notification: " << msg;
}

void RobotParticipant::gameStartedSlot(Participant::ParticipantSideEnum nextOnTurn, QString stepsSoFar)
{
    executeJoinedSteps(stepsSoFar);
    QStringList steps = stepsSoFar.split(';');
    QString lastStep = steps[steps.length() - 1];
    if(isPlayerSide(nextOnTurn)) makeNextStep(lastStep);
}

void RobotParticipant::stepCalculationDoneSlot(QString step)
{
    lastStepByRobot = step;
    emit stepInitiatedSignal(step);
}

void RobotParticipant::makeNextStep(QString lastStep)
{
    RobotStepCalculator* stepCalculator = new RobotStepCalculator(board->getActiveBoard(), pSide, lastStep);
    stepCalculator->setAutoDelete(true);
    connect(stepCalculator, SIGNAL(stepCalculated(QString)),
            this, SLOT(stepCalculationDoneSlot(QString)), Qt::QueuedConnection);

    pool->start(stepCalculator);
}

ValidatorBase *RobotParticipant::findValidator(QChar piece)
{
    ValidatorBase* res = nullptr;
    for(int i = 0; i < validators.length(); i++){
        if(validators[i]->isValidatorsResponsibility(piece)){
            res = validators[i];
            break;
        }
    }
    return res;
}

void RobotParticipant::executeJoinedSteps(QString joinedSteps)
{
    QStringList stepList = joinedSteps.split(";");
    for(int i = 0; i < stepList.length(); i++){
        if(stepList[i] == "") break;
        board->executeStep(stepList[i]);
    }
}

void RobotParticipant::stepHappenedSlot(QString step, Participant::ParticipantSideEnum newTurnColor)
{
    if(step == "") return;
    board->executeStep(step);

    if(newTurnColor == pSide){
        makeNextStep(step);
    }
}

void RobotParticipant::undoNeedsApprovalSlot(Participant::ParticipantSideEnum approvingSideColor)
{
    if(pSide != approvingSideColor) return;
    emit approveUndoSignal();
}

void RobotParticipant::undoHappenedSlot(QString newStepsSoFar, Participant::ParticipantSideEnum nextC)
{
    board->restartBoard();
    executeJoinedSteps(newStepsSoFar);
    if(pSide == nextC){
        QStringList steps = newStepsSoFar.split(';');
        QString lastStep = steps[steps.length() - 1];
        makeNextStep(lastStep);
    }
}
