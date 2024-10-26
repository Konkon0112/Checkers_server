#include "gamemodel.h"
#include "validatorpawn.h"
#include "validatordame.h"
#include "customexception.h"
#include "ToastTypeEnum.h"

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

void GameModel::startGame()
{
    state = GameState::ACTIVE;
    updateUseablePieces();
}

void GameModel::restartGame()
{
    colorOnTurn = Participant::ParticipantSideEnum::LIGHT;
    state = GameState::ACTIVE;
    board->restartBoard();
    stepList.clear();
    updateUseablePieces();
}

void GameModel::passStepForward(QString step)
{
    if(state == GameState::FINISHED) throw new CustomException("The game is already finished.", ToastTypeEnum::ERROR);
    if(step == "") throw new CustomException("A step must do something.", ToastTypeEnum::WARNING);

    QStringList stepDissasembled = step.split('-');
    if(stepDissasembled.length() == 1){
        stepDissasembled = step.split('x');
    }
    int from = stepDissasembled[0].toInt();
    int to = stepDissasembled[1].toInt();
    QChar charAtInd = board->charAtIndex(from);

    if(charAtInd == 'x') throw new CustomException("Empty square selected.", ToastTypeEnum::WARNING);
    if(colorOnTurn == Participant::ParticipantSideEnum::LIGHT && charAtInd.isLower()) throw new CustomException("Not your piece.", ToastTypeEnum::WARNING);
    if(colorOnTurn == Participant::ParticipantSideEnum::DARK && charAtInd.isUpper()) throw new CustomException("Not your piece.", ToastTypeEnum::WARNING);

    ValidatorBase* validator = nullptr;
    for(int i = 0; i < validators.size(); i++){
        if(validators.at(i)->isValidatorsResponsibility(board->charAtIndex(from))) {
            validator = validators[i];
            break;
        }
    }

    if(!validator) throw new CustomException("No validator found.", ToastTypeEnum::ERROR);
    if(!useablePieces.contains(from)) throw new CustomException("You cannot step with this piece.", ToastTypeEnum::WARNING);
    if(!validator->isValidStep(step, board->getActiveBoard())) throw new CustomException("Not a valid step.", ToastTypeEnum::WARNING);
    // The order of these two matters!
    addStepToList(step);
    board->executeStep(step);

    // check for turn change
    Participant::ParticipantSideEnum newTurn =
        colorOnTurn == Participant::ParticipantSideEnum::DARK?
            Participant::ParticipantSideEnum::LIGHT: Participant::ParticipantSideEnum::DARK;

    bool isGameOver = checkIfGameOver(newTurn);
    if(isGameOver) return;

    Participant::ParticipantSideEnum nextColor;

    if(step.contains('x')){
        QSet<QString> possibleSteps = validator->getValidIndecies(to, board->getActiveBoard());

        if(possibleSteps.empty()){ // If piece can't step after take
            nextColor = newTurn;
            completeTasksOnTurnChange(newTurn);

        } else {
            QString firstPos = *possibleSteps.begin();

            // If a piece can capture, then it has to capture
            // So if one contains '-' that means all of the steps are normal steps
            if(firstPos.contains('-')){
                nextColor = newTurn;
                completeTasksOnTurnChange(newTurn);

            } else { //It can perform chained capture
                updateUseablePieces(to);
                nextColor = colorOnTurn;
            }
        }


    } else { // If the last move was not capture
        nextColor = newTurn;
        completeTasksOnTurnChange(newTurn);
    }
    if(state == GameState::ACTIVE) emit stepHappenedSignal(step, nextColor);

}

void GameModel::passStepOnSetUp(QString step)
{
    if(state != GameState::UNDER_STEUP) return;

    // The order matters, because the addStepToList() makes calculation
    // based on the board state before moving
    addStepToList(step);
    board->executeStep(step);
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

    QList<QPair<Participant::ParticipantSideEnum, QStringList>> dummyList =
        stepList.sliced(0, indOfLastStepSequence);

    QStringList dummyToJoin;

    for(int i = 0; i < dummyList.length(); i++){
        dummyToJoin.append(dummyList.at(i).second.join(';'));
    }
    QString joinedSteps = dummyToJoin.join(';');
    restartGame();

    setUpContinuedGame(joinedSteps, playerWhoInitiated);
    stepList = dummyList;

    colorOnTurn = playerWhoInitiated;

    emit undoHappenedSignal(joinedSteps, playerWhoInitiated);
    updateUseablePieces();
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
}

void GameModel::setUpContinuedGame(QString stepsSoFar, Participant::ParticipantSideEnum nextColor)
{
    state = GameState::UNDER_STEUP;
    QStringList stepsHappened = stepsSoFar.split(';');
    for(int i = 0; i < stepsHappened.length(); i++){
        if(stepsHappened.at(i) == "") break;
        passStepOnSetUp(stepsHappened.at(i));
    }
    colorOnTurn = nextColor;
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
}

void GameModel::addStepToList(QString step)
{
    QStringList stepDissasembled = step.split('-');
    if(stepDissasembled.length() == 1) {
        stepDissasembled = step.split('x');
    }

    // Determine piece color based on the color of the piece the step is moving
    int from = stepDissasembled[0].toInt();
    Participant::ParticipantSideEnum pS = board->getActiveBoard().at(from).isLower()?
        Participant::ParticipantSideEnum::DARK:
        Participant::ParticipantSideEnum::LIGHT;

    if(stepList.isEmpty()) {
        QStringList steps;
        steps.append(step);
        QPair<Participant::ParticipantSideEnum, QStringList> pair(pS, steps);
        stepList.append(pair);

    } else {
        if(stepList[stepList.length() - 1].first == pS) {
            stepList[stepList.length() - 1].second.append(step);

        } else {
            QStringList steps;
            steps.append(step);
            QPair<Participant::ParticipantSideEnum, QStringList> pair(pS, steps);
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
