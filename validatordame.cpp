#include "validatordame.h"

ValidatorDame::ValidatorDame(QObject *parent)
    : ValidatorBase{parent}
{
    validationChars.first = 'd';
    validationChars.second = 'D';
}

QSet<QString> ValidatorDame::getValidSteps(int startInd, QString board)
{
    QList<QPair<int, int>> l;
    l.append(QPair<int, int>(-1, -1));
    l.append(QPair<int, int>(1, -1));
    l.append(QPair<int, int>(-1, 1));
    l.append(QPair<int, int>(1, 1));

    return this->getValidStepIndecies(startInd, l, 8, board);
}
