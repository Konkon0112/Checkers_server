#include <QStringList>
#include <QSet>
#include <QRandomGenerator>

#include "robotparticipant.h"
#include "validatordame.h"
#include "validatorpawn.h"

RobotParticipant::RobotParticipant(ParticipantSideEnum pS, QObject *parent)
    : Participant{ParticipantTypeEnum::PLAYER, pS, parent}
{
    board = new Board(this);
    validators.append(new ValidatorPawn(this));
    validators.append(new ValidatorDame(this));
}

bool RobotParticipant::usingThisSocket(QTcpSocket *soc)
{
    return false;
}

void RobotParticipant::gameStartedSlot(Participant::ParticipantSideEnum nextOnTurn, QString stepsSoFar)
{
    executeJoinedSteps(stepsSoFar);
    if(isPlayerSide(nextOnTurn)) makeNextStep();
}

void RobotParticipant::makeNextStep()
{
    QString activeBoard = board->getActiveBoard();
    QStringList possibleNormalSteps;
    QStringList possibleTakingSteps;

    for(int i = 0; i < activeBoard.length(); i++){
        if(activeBoard[i] == 'x') continue;

        Participant::ParticipantSideEnum charSide = activeBoard[i].isLower()?
            Participant::ParticipantSideEnum::DARK:
            Participant::ParticipantSideEnum::LIGHT;
        bool isRobotPiece = isPlayerSide(charSide);
        if(!isRobotPiece) continue;

        ValidatorBase* val = findValidator(activeBoard[i]);
        if(!val) continue;
        QSet<QString> posStepSet = val->getValidIndecies(i, activeBoard);

        for (auto i = posStepSet.cbegin(), end = posStepSet.cend(); i != end; ++i){
            QString stepValue = *i;
            if(stepValue.contains('x')){
                possibleTakingSteps.append(stepValue);
            } else {
                possibleNormalSteps.append(stepValue);
            }
        }
    }
    QStringList finalList = possibleTakingSteps.length() == 0?
                                possibleNormalSteps : possibleTakingSteps;

    // Step randomly for now
    int stepInd = QRandomGenerator::global()->bounded(finalList.length());
    emit stepInitiatedSignal(finalList[stepInd]);
    lastStepMade = finalList[stepInd];
}

void RobotParticipant::makeChainedStep(QString lastStep)
{
    QString activeBoard = board->getActiveBoard();
    QStringList stepDissasembled = lastStep.split('x');
    if(stepDissasembled.length() == 0) return;
    int to = stepDissasembled[1].toInt();

    ValidatorBase* val = findValidator(activeBoard[to]);
    if(!val) return;

    QSet<QString> posStepSet = val->getValidIndecies(to, activeBoard);

    QStringList possibleNormalSteps;
    QStringList possibleTakingSteps;

    for (auto i = posStepSet.cbegin(), end = posStepSet.cend(); i != end; ++i){
        QString stepValue = *i;
        if(stepValue.contains('x')){
            possibleTakingSteps.append(stepValue);
        } else {
            possibleNormalSteps.append(stepValue);
        }
    }

    QStringList finalList = possibleTakingSteps.length() == 0?
                                possibleNormalSteps : possibleTakingSteps;

    // Step randomly for now
    int stepInd = QRandomGenerator::global()->bounded(finalList.length());
    emit stepInitiatedSignal(finalList[stepInd]);
    lastStepMade = finalList[stepInd];
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

    if(newTurnColor == pSide && step == lastStepMade){
        qInfo() << "chained";
        makeChainedStep(step);
    } else if (newTurnColor == pSide){
        makeNextStep();
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
        makeNextStep();
    }
}
