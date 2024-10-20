#include <QtMath>
#include "evaluatordame.h"

EvaluatorDame::EvaluatorDame(QObject *parent)
    : EvaluatorBase{'D', 'd', 3, parent}
{
    dameValidator = new ValidatorDame(this);
}

float EvaluatorDame::evaluatePiece(int ind, QString board, QString lastStep)
{
    float result = baseVal;
    bool isDark = board[ind].isLower();

    result += addEdgeOfBoardBonus(ind);
    result += addNumOfTargetBonus(ind, board, lastStep);

    return isDark? -result : result;
}

float EvaluatorDame::addEdgeOfBoardBonus(int ind)
{
    float res = 0;
    int x = ind % 8;
    int y = ind / 8;

    if(x == 0 || x == 7 || y == 0 || y == 7) res += 0.5;

    return res;
}

float EvaluatorDame::addNumOfTargetBonus(int ind, QString board, QString lastStep)
{
    float res = 0;
    bool isChained;

    if(lastStep.contains('x')){
        QStringList sL = lastStep.split('x');
        isChained = sL[1].toInt() == ind;
    } else {
        isChained = false;
    }

    QSet<QString> pSteps = dameValidator->getValidIndecies(ind, board);
    for (auto i = pSteps.cbegin(), end = pSteps.cend(); i != end; ++i){
        QString stepValue = *i;

        // There is a normal step, that means there is no taking step
        // because of the forced taking
        if(stepValue.contains('-')) return 0;

        QStringList stepDissasembled = stepValue.split('x');
        int to = stepDissasembled[1].toInt();

        int indOfTarget = getIndOfTarget(ind, to);

        if(isChained){
            if(board[indOfTarget] == 'd' || board[indOfTarget] == 'D'){
                res += 2; // This means we can take dame from here
            } else {
                res += 1;
            }
        } else {
            if(board[indOfTarget] == 'd' || board[indOfTarget] == 'D'){
                // This means we cannot take dame from here and
                // our dame will probably be taken in the next turn
                res -= 3;
            } else {
                res += 1;
            }
        }
    }

    return res;
}

int EvaluatorDame::getIndOfTarget(int from, int to)
{
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

    return tX * 8 + tY;
}
