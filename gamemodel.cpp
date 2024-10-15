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
    state = GameState::UNDER_STEUP;

    useablePieceFinder = new UseablePieceFinder(this);
}

void GameModel::restartGame()
{
    colorOnTurn = Participant::ParticipantSideEnum::LIGHT;
    state = GameState::ACTIVE;
    updateUseablePieces();
}

void GameModel::passStepForward(QString step)
{
    if(state == GameState::FINISHED) return;
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

            if(useablePieces.contains(from) &&
                validators.at(i)->isValidStep(step, board->getActiveBoard())){

                board->executeStep(step);
                addStepToList(step);
                if(state == GameState::ACTIVE) emit stepHappenedSignal(step);
                // check for turn change
                Participant::ParticipantSideEnum newTurn =
                    colorOnTurn == Participant::ParticipantSideEnum::DARK?
                        Participant::ParticipantSideEnum::LIGHT: Participant::ParticipantSideEnum::DARK;

                bool isGameOver = checkIfGameOver(newTurn);
                if(isGameOver) return;

                if(step.contains('x')){
                    QSet<QString> possibleSteps = validators.at(i)->getValidIndecies(to, board->getActiveBoard());
                    if(possibleSteps.empty()){
                        completeTasksOnTurnChange(newTurn);
                        return;
                    }
                    QString firstPos = *possibleSteps.begin();

                    // If a piece can capture, then it has to capture
                    // So if one contains '-' that means all of the steps are normal steps
                    if(firstPos.contains('-')){
                        completeTasksOnTurnChange(newTurn);

                    } else { //It can perform chained capture
                        updateUseablePieces(to);
                    }


                } else {
                    completeTasksOnTurnChange(newTurn);
                }
            }
            break;
        }
    }
}

void GameModel::undoStep(Participant::ParticipantSideEnum playerWhoInitiated)
{
    if(stepList.length() == 0) return;

    int indOfLastStepSequence = -1;

    for(int i = stepList.length() - 1; i >= 0; i--){
        if(stepList.at(i).first == playerWhoInitiated){
            indOfLastStepSequence = i;
            break;
        }
    }

    if(indOfLastStepSequence == -1) return;

    stepList = stepList.sliced(0, indOfLastStepSequence);

    restartGame();
    setUpContinuedGame(getJoinedStepStr());

    emit undoHappenedSignal(getJoinedStepStr());

    setColorOnTurn(playerWhoInitiated);
}

Participant::ParticipantSideEnum GameModel::getColorOnTurn() const
{
    return colorOnTurn;
}

QString GameModel::getJoinedStepStr()
{
    QStringList res;
    for(int i = 0; i < stepList.length(); i++){
        res.append(stepList.at(i).second.join(';'));
    }
    return res.join(';');
}

void GameModel::setColorOnTurn(Participant::ParticipantSideEnum newColorOnTurn)
{
    colorOnTurn = newColorOnTurn;
    emit turnChangedSignal(newColorOnTurn);
}

void GameModel::setUpContinuedGame(QString stepsSoFar)
{
    state = GameState::UNDER_STEUP;
    QStringList stepsHappened = stepsSoFar.split(';');
    for(int i = 0; i < stepsHappened.length(); i++){
        if(stepsHappened.at(i) == "") break;
        passStepForward(stepsHappened.at(i));
    }
    state = GameState::ACTIVE;
}

bool GameModel::checkIfGameOver(Participant::ParticipantSideEnum playerOnTurnSide)
{
    QString activeBoard = board->getActiveBoard();
    for(int i = 0; i < activeBoard.length(); i++){
        ValidatorBase* v = nullptr;
        for(int j = 0; j < validators.size(); j++){
            if(validators.at(j)->isValidatorsResponsibility(board->charAtIndex(i))) v = validators.at(j);
        }
        if(v == nullptr) continue;
        if(playerOnTurnSide == Participant::ParticipantSideEnum::DARK &&
                activeBoard.at(i).isLower()){
            QSet<QString> possibleSteps = v->getValidIndecies(i, activeBoard);
            if(!possibleSteps.isEmpty()) return false;

        } else if(playerOnTurnSide == Participant::ParticipantSideEnum::LIGHT &&
                   activeBoard.at(i).isUpper()){
            QSet<QString> possibleSteps = v->getValidIndecies(i, activeBoard);
            if(!possibleSteps.isEmpty()) return false;
        }
    }

    Participant::ParticipantSideEnum winner =
        playerOnTurnSide == Participant::ParticipantSideEnum::DARK?
            Participant::ParticipantSideEnum::LIGHT: Participant::ParticipantSideEnum::DARK;

    if(state == GameState::ACTIVE) emit gameOverSignal(winner);

    state = GameState::FINISHED;
    return true;
}

void GameModel::completeTasksOnTurnChange(Participant::ParticipantSideEnum playerOnTurnSide)
{
    colorOnTurn = playerOnTurnSide;
    updateUseablePieces();
    if(state == GameState::ACTIVE){
        emit turnChangedSignal(playerOnTurnSide);
    }
}

void GameModel::addStepToList(QString step)
{
    if(stepList.isEmpty()) {
        QStringList steps;
        steps.append(step);
        QPair<Participant::ParticipantSideEnum, QStringList> pair(colorOnTurn, steps);
        stepList.append(pair);

    } else {
        QPair<Participant::ParticipantSideEnum, QStringList> lastStepItem = stepList.last();
        if(lastStepItem.first == colorOnTurn) {
            lastStepItem.second.append(step);

        } else {
            QStringList steps;
            steps.append(step);
            QPair<Participant::ParticipantSideEnum, QStringList> pair(colorOnTurn, steps);
            stepList.append(pair);

        }
    }
}

void GameModel::updateUseablePieces(int chainInd)
{
    if(chainInd != -1){
        useablePieces.clear();
        useablePieces.insert(chainInd);
    } else {
        useablePieces =
            useablePieceFinder->findUseablePieceIndecies(board->getActiveBoard(), colorOnTurn);
    }
}
