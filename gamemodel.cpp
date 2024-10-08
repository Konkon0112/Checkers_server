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
    int to = stepDissasembled[1].toInt();
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

                Participant::ParticipantSideEnum newTurn =
                    colorOnTurn == Participant::ParticipantSideEnum::DARK?
                        Participant::ParticipantSideEnum::LIGHT: Participant::ParticipantSideEnum::DARK;

                if(step.contains('x')){
                    QSet<QString> possibleSteps = validators.at(i)->getValidIndecies(to, board->getActiveBoard());
                    if(possibleSteps.empty()){
                        emit turnChangedSignal(newTurn);
                        colorOnTurn = newTurn;
                    }
                    QString firstPos = *possibleSteps.begin();

                    // If a piece can hit, then it has to hit
                    // So if one contains '-' that means all of the steps are normal steps
                    if(firstPos.contains('-')){
                        emit turnChangedSignal(newTurn);
                        colorOnTurn = newTurn;
                    }


                } else {
                    emit turnChangedSignal(newTurn);
                    colorOnTurn = newTurn;
                    qInfo() << "Changing turn";
                }
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
