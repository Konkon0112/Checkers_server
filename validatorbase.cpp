#include "validatorbase.h"

ValidatorBase::ValidatorBase(QObject *parent)
    : QObject{parent}
{}

bool ValidatorBase::isValidatorsResponsibility(QChar c)
{
    return c == validationChars.first || c == validationChars.second;
}

bool ValidatorBase::isValidStep(QString step, QString board)
{
    QStringList stepDissasembled = step.split('-');
    if(stepDissasembled.length() == 1){
        stepDissasembled = step.split('x');
    }
    int from = stepDissasembled[0].toInt();
    QSet<QString> validSteps = getValidIndecies(from, board);

    for (auto i = validSteps.cbegin(), end = validSteps.cend(); i != end; ++i){
        QString stepValue = *i;
        if(stepValue == step) return true;
    }
    return false;
}

QSet<QString> ValidatorBase::getValidStepIndecies(int startingInd, QList<QPair<int, int> > dirPair, int maxIteration, QString board)
{
    int sRow = startingInd / 8;
    int sCol = startingInd % 8;
    // We need coordinates because there are cases where a step
    // would not pass the edge indecies (not <0 or >63), but it would
    // still be an unreasonable step
    // For example: Left column in the direction of North-West
    QSet<QString> validStepSet;
    for(int i = 0; i < dirPair.count(); i++){
        int dX = dirPair.at(i).first;
        int dY = dirPair.at(i).second;
        getSteps(sRow, sCol, dX, dY, maxIteration, board, validStepSet);
    }

    QSet<QString> pieceTakingStepSet;
    QSet<QString> simpleStepSet;
    for (auto i = validStepSet.cbegin(), end = validStepSet.cend(); i != end; ++i){
        QString step = *i;
        if(step.contains("x")){
            pieceTakingStepSet.insert(step);
        } else {
            simpleStepSet.insert(step);
        }
    }

    if(pieceTakingStepSet.count() != 0){
        return pieceTakingStepSet;
    }
    return simpleStepSet;
}

void ValidatorBase::getSteps(int sRow, int sCol, int dX, int dY, int maxIteration, QString board, QSet<QString> &validContainer)
{
    int sInd = 8 * sRow + sCol;
    QChar sChar = board.at(sInd);

    for(int i = 0; i < maxIteration; i++){
        sRow += dY;
        sCol += dX;
        if(coordsAreOutsideTheBoard(sCol, sRow)){
            break;
        }
        int indInStr = 8 * sRow + sCol;
        QChar currentChar = board.at(indInStr);
        if(currentChar == 'x'){
            validContainer.insert(QString::number(sInd) + "-" + QString::number(indInStr));
        } else if (!isSameTeam(currentChar, sChar)) {
            checkIfPieceTakeable(sCol, sRow, dX, dY, sInd, board, validContainer);
            break;
        } else { // Same team piece
            break;
        }
    }
}

void ValidatorBase::checkIfPieceTakeable(int tCol, int tRow, int dX, int dY, int sInd, QString board, QSet<QString> &validContainer)
{
    tCol += dX;
    tRow += dY;
    if(coordsAreOutsideTheBoard(tCol, tRow)) return;

    int ind = 8 * tRow + tCol;

    if(board.at(ind) == 'x'){
        validContainer.insert(QString::number(sInd) + "x" + QString::number(ind));
    }
}

bool ValidatorBase::isSameTeam(QChar c1, QChar c2)
{
    return (c1.isUpper() && c2.isUpper()) ||
           (c1.isLower() && c2.isLower());
}

bool ValidatorBase::coordsAreOutsideTheBoard(int x, int y)
{
    return (x < 0) || (x > 7) || (y < 0) || (y > 7);
}
