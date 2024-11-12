#include <QStringList>
#include <limits>

#include "robotstepcalculator.h"
#include "validatordame.h"
#include "validatorpawn.h"

RobotStepCalculator::RobotStepCalculator(QString board, Participant::ParticipantSideEnum robotS, QString lStep, QObject *parent)
: QObject{parent}
{
    isMaximizingPlayer = robotS == Participant::ParticipantSideEnum::LIGHT;
    this->board = board;
    evaluator = new PositionEvaluator(this);
    validators.append(new ValidatorDame(this));
    validators.append(new ValidatorPawn(this));
    lastStep = lStep;
}

void RobotStepCalculator::run()
{
    qInfo() << "Robot calculating...";
    QPair<float, QString> dummyAlpha(std::numeric_limits<float>::min(), "");
    QPair<float, QString> dummyBeta(std::numeric_limits<float>::max(), "");

    QPair<float, QString> res = minimax(board,
                                        lastStep,
                                        searchDepth,
                                        dummyAlpha,
                                        dummyBeta,
                                        isMaximizingPlayer);

    qInfo() << "Calc done: " << res.second;
    emit stepCalculated(res.second);
}

QString RobotStepCalculator::makeMoveOnBoard(QString board, QString step)
{
    QStringList stepDissasembled = step.split('-');
    if(stepDissasembled.length() == 1){
        stepDissasembled = step.split('x');
    }

    int from = stepDissasembled[0].toInt();
    int to = stepDissasembled[1].toInt();

    board[to] = board[from];
    board[from] = 'x';

    if(step.contains('x')){
        int sX = from / 8;
        int sY = from % 8;
        int tX = to / 8;
        int tY = to % 8;

        int dX = sX - tX;
        int dY = sY - tY;

        dX = dX / qFabs<int>(dX);
        dY = dY / qFabs<int>(dY);

        tX = tX + dX;
        tY = tY + dY;

        int tIndInStr = tX * 8 + tY;

        board[tIndInStr] = 'x';
    }

    if(board[to] == 'p' && to >= 56){
        board.replace(to, 1, 'd');

    } else if(board[to] == 'P' && to <= 7) {
        board.replace(to, 1, 'D');
    }
    return board;
}

QPair<float, QString> RobotStepCalculator::minimax(QString board, QString lastStep, int depth, QPair<float, QString> &alpha, QPair<float, QString> &beta, bool isMaxPlayer)
{
    QList<QString> possibleSteps = getAllPossibleSteps(board, isMaxPlayer, lastStep);
    float currentPosValue = evaluator->evaluatePosition(board, lastStep);

    if(possibleSteps.length() == 0){ // Game over for the next player
        // Depending on max step was made by whom change value
        QPair<float, QString> res;
        res.first = isMaxPlayer? std::numeric_limits<float>::min() : std::numeric_limits<float>::max();
        res.second = lastStep;
        return res;
    }
    if(depth == 0){ //Reached max depth
        QPair<float, QString> res;
        res.first = currentPosValue;
        res.second = lastStep;
        return res;
    }


    if(isMaxPlayer){
        QPair<float, QString> maxEval;
        maxEval.first = std::numeric_limits<float>::min();
        maxEval.second = lastStep;
        for(int i = 0; i < possibleSteps.length(); i++){

            QString newPos = makeMoveOnBoard(board, possibleSteps[i]);
            bool isChainingPosible = isChainedPossible(possibleSteps[i], newPos);

            QPair<float, QString> eval;
            if(isChainingPosible){
                eval = minimax(newPos, possibleSteps[i], depth -1, alpha, beta, isMaxPlayer);
            } else {
                eval = minimax(newPos, possibleSteps[i], depth -1, alpha, beta, !isMaxPlayer);
            }
            maxEval = maxVal(maxEval, eval);
            alpha = maxVal(alpha, eval);
            if(beta.first <= alpha.first){
                break;
            }
        }
        if(depth != searchDepth) maxEval.second = lastStep;
        return maxEval;
    } else {
        QPair<float, QString> minEval;
        minEval.first = std::numeric_limits<float>::max();
        minEval.second = lastStep;
        for(int i = 0; i < possibleSteps.length(); i++){
            QString newPos = makeMoveOnBoard(board, possibleSteps[i]);
            bool isChainingPosible = isChainedPossible(possibleSteps[i], newPos);

            QPair<float, QString> eval;
            if(isChainingPosible){
                eval = minimax(newPos, possibleSteps[i], depth -1, alpha, beta, isMaxPlayer);
            } else {
                eval = minimax(newPos, possibleSteps[i], depth -1, alpha, beta, !isMaxPlayer);
            }
            minEval = minVal(minEval, eval);
            beta = minVal(beta, eval);
        }
        if(depth != searchDepth) minEval.second = lastStep;

        return minEval;
    }
}

QPair<float, QString> RobotStepCalculator::maxVal(QPair<float, QString> x, QPair<float, QString> y)
{
    return x.first > y.first? x : y;
}

QPair<float, QString> RobotStepCalculator::minVal(QPair<float, QString> x, QPair<float, QString> y)
{
    return x.first < y.first? x : y;
}

ValidatorBase *RobotStepCalculator::getValidator(QChar square)
{
    for(int i = 0; i < validators.length(); i++){
        if(validators[i]->isValidatorsResponsibility(square)) return validators[i];
    }
    return nullptr;
}

QList<QString> RobotStepCalculator::getAllPossibleSteps(QString board, bool isMaxPlayer, QString lastStep)
{
    QList<QString> normalSteps;
    QList<QString> pieceTakingSteps;

    if(lastStep.contains('x')){
        QStringList lastStepDissasembled = lastStep.split('x');
        int to = lastStepDissasembled[1].toInt();
        bool isSamePlayer = (isMaxPlayer && board[to].isUpper()) ||
                            (!isMaxPlayer && board[to].isLower());
        if(isSamePlayer){
            ValidatorBase* validator = getValidator(board[to]);
            if(!validator) normalSteps; // empty

            QSet<QString> pSteps = validator->getValidSteps(to, board);

            for (auto i = pSteps.cbegin(), end = pSteps.cend(); i != end; ++i){
                QString stepValue = *i;
                if(stepValue.contains('x')){
                    pieceTakingSteps.append(stepValue);
                } else {
                    normalSteps.append(stepValue);
                }
            }

            return pieceTakingSteps.length() == 0? normalSteps:pieceTakingSteps;
        }
    }


    for(int i = 0; i < board.length(); i++){

        if((isMaxPlayer&&board[i].isLower()) || (!isMaxPlayer&&board[i].isUpper())) continue;

        ValidatorBase* validator = getValidator(board[i]);
        if(!validator) continue;

        QSet<QString> pSteps = validator->getValidSteps(i, board);

        for (auto i = pSteps.cbegin(), end = pSteps.cend(); i != end; ++i){
            QString stepValue = *i;
            if(stepValue.contains('x')){
                pieceTakingSteps.append(stepValue);
            } else {
                normalSteps.append(stepValue);
            }
        }
    }

    return pieceTakingSteps.length() == 0? normalSteps:pieceTakingSteps;
}

bool RobotStepCalculator::isChainedPossible(QString lastStep, QString board)
{
    QStringList stepDisasmbled = lastStep.split('-');
    if(stepDisasmbled.length() != 1){
        return false;
    } else {
        stepDisasmbled = lastStep.split('x');
    }

    int to = stepDisasmbled[1].toInt();

    ValidatorBase* validator = getValidator(board[to]);
    QSet<QString> pSteps = validator->getValidSteps(to, board);

    if(pSteps.isEmpty()) return false;

    QString first = *(pSteps.cbegin());

    return first.contains('x');
}
































