#include "validatorpawn.h"

ValidatorPawn::ValidatorPawn(QObject *parent)
    : ValidatorBase{parent}
{
    validationChars.first = 'p';
    validationChars.second = 'P';
}

QSet<QString> ValidatorPawn::getValidIndecies(int startInd, QString board)
{
    QChar c = board.at(startInd);
    QList<QPair<int, int>> l;
    if(c == 'p'){ //DARK
        l.append(QPair<int, int>(-1, 1));
        l.append(QPair<int, int>(1, 1));
    } else if(c == 'P') { //LIGHT
        l.append(QPair<int, int>(-1, -1));
        l.append(QPair<int, int>(1, -1));
    }
    return this->getValidStepIndecies(startInd, l, 1, board);
}
