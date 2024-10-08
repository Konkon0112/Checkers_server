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
    for(int i = 0; i < validators.size(); i++){
        if(validators.at(i)->isValidatorsResponsibility(board->charAtIndex(from))){
            if(validators.at(i)->isValidStep(step, board->getActiveBoard())){
                board->executeStep(step);
                emit stepHappenedSignal(step);
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
