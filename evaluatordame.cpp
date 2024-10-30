#include <QtMath>
#include "evaluatordame.h"

EvaluatorDame::EvaluatorDame(QObject *parent)
    : EvaluatorBase{'D', 'd', 3, 1, parent}
{}

float EvaluatorDame::evaluatePiece(int ind, QString board, QString lastStep)
{
    if(board[ind] == 'x') return 0;
    float result = baseVal;
    bool isDark = board[ind].isLower();

    result += addEdgeOfBoardBonus(ind);
    result += addNumOfTargetBonus(ind, board, lastStep);
    result += underAttackSubBonus(ind, board, lastStep, result);

    return isDark? -result : result;
}

float EvaluatorDame::addEdgeOfBoardBonus(int ind)
{
    float res = 0;
    int x = ind % 8;
    int y = ind / 8;

    if(x == 0 || x == 7 || y == 0 || y == 7) res += 1;

    return res;
}

float EvaluatorDame::addNumOfTargetBonus(int ind, QString board, QString lastStep)
{
    float res = 0;

    QSet<QString> pSteps = validators[valIndex]->getValidIndecies(ind, board);
    for (auto i = pSteps.cbegin(), end = pSteps.cend(); i != end; ++i){
        QString stepValue = *i;

        // There is a normal step, that means there is no taking step
        // because of the forced taking
        if(stepValue.contains('-')) return 0;

        QStringList stepDissasembled = stepValue.split('x');
        int to = stepDissasembled[1].toInt();

        int indOfTarget = getIndOfTarget(ind, to);

        if(board[indOfTarget] == 'd' || board[indOfTarget] == 'D'){
            res += 2;
        } else {
            res += 1;
        }

    }

    return res;
}
