#include <QStringList>
#include <limits>

#include "robotstepcalculator.h"

RobotStepCalculator::RobotStepCalculator(QString board, Participant::ParticipantSideEnum robotS, QObject *parent)
: QObject{parent}
{
    isMaximizingPlayer = robotS == Participant::ParticipantSideEnum::LIGHT;
    this->board = board;
    evaluator = new PositionEvaluator(this);
}

void RobotStepCalculator::run()
{

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

    if(sl[i].contains('x')){
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

        resultBoard[tIndInStr] = 'x';
    }

    if(board[to] == 'p' && to >= 56){
        board.replace(to, 1, 'd');

    } else if(board[to] == 'P' && to <= 7) {
        board.replace(to, 1, 'D');
    }
    return board;
}

int RobotStepCalculator::minimax(QString board, int depth, int &alpha, int &beta, bool isMaxPlayer)
{
    //Count in the chained attack! Most nincs kedvem hozzá ;)
    // Ha a legutolsó lépést is tovább passzoljuk akkor kitalálható, hogy
    // chained vagy sem
    QList<QString> possibleSteps;
    if(depth == 0){
        return evaluator->evaluatePosition(board);
    }

    if(possibleSteps.length() == 0){
        return isMaxPlayer?
            std::numeric_limits<float>::min() : std::numeric_limits<float>::max();
    }

    if(isMaxPlayer){
        int maxEval = std::numeric_limits<float>::min();
        for(int i = 0; i < possibleSteps.length(); i++){
            QString newPos = makeMoveOnBoard(board, possibleSteps[i]);
            int eval = minimax(newPos, depth -1, alpha, beta, !isMaxPlayer);
            maxEval = maxVal(maxEval, eval);
            alpha = maxVal(alpha, eval);
            if(beta <= alpha){
                break;
            }
        }
        return maxEval;
    } else {
        int minEval = std::numeric_limits<float>::max();
        for(int i = 0; i < possibleSteps.length(); i++){
            QString newPos = makeMoveOnBoard(board, possibleSteps[i]);
            int eval = minimax(newPos, depth -1, alpha, beta, !isMaxPlayer);
            minEval = minVal(minEval, eval);
            beta = min(beta, eval);
        }
        return minEval;
    }
}

int RobotStepCalculator::maxVal(int x, int y)
{
    return x > y? x : y;
}

int RobotStepCalculator::minVal(int x, int y)
{
    return x < y? x : y;
}
