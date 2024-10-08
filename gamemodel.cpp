#include "gamemodel.h"
#include "validatorpawn.h"
#include "validatordame.h"

GameModel::GameModel(QObject *parent)
    : QObject{parent}
{
    board = new Board(this);
    colorOnTurn = Participant::ParticipantSideEnum::LIGHT;

    validators.append(new ValidatorPawn(this));
    validators.append(new ValidatorDame(this));
}

void GameModel::resetGame()
{
    board->restartBoard();
    colorOnTurn = Participant::ParticipantSideEnum::LIGHT;
}

void GameModel::passStepForward(QString step)
{
    QStringList stepDissasembled = step.split('-');
    if(stepDissasembled.length() == 1){
        stepDissasembled = step.split('x');
    }
    int from = stepDissasembled[0].toInt();
    QChar charAtInd = board->charAtIndex(from);

    if(charAtInd == 'x') return;
    if(colorOnTurn == Participant::ParticipantSideEnum::LIGHT && charAtInd.isLower()) return;
    if(colorOnTurn == Participant::ParticipantSideEnum::DARK && charAtInd.isUpper()) return;

    for(int i = 0; i < validators.size(); i++){
        if(validators.at(i)->isValidatorsResponsibility(board->charAtIndex(from))){
            if(validators.at(i)->isValidStep(step, board->getActiveBoard())){
                board->executeStep(step);
                stepList.append(step);
                emit stepHappenedSignal(step);
                // check for turn change
            }
            break;
        }
    }
}

void GameModel::undoStep(Participant::ParticipantSideEnum playerWhoInitiated)
{

}

Participant::ParticipantSideEnum GameModel::getColorOnTurn() const
{
    return colorOnTurn;
}

QString GameModel::getJoinedStepStr()
{
    return stepList.join(';');
}

void GameModel::setColorOnTurn(Participant::ParticipantSideEnum newColorOnTurn)
{
    colorOnTurn = newColorOnTurn;
}
