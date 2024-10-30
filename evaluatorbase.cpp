#include "evaluatorbase.h"
#include "validatordame.h"
#include "validatorpawn.h"

EvaluatorBase::EvaluatorBase(QChar lightCh, QChar darkCh, float baseV, int validatorInd, QObject *parent)
    : QObject{parent}
{
    lightChar = lightCh;
    darkChar = darkCh;
    baseVal = baseV;
    valIndex = validatorInd;

    validators.append(new ValidatorPawn(this));
    validators.append(new ValidatorDame(this));
}

bool EvaluatorBase::isEvaluatorResponsibility(QChar ch)
{
    return ch == lightChar || ch == darkChar;
}

ValidatorBase *EvaluatorBase::findValidator(QChar ch)
{
    for(int i = 0; i < validators.length();i ++){
        if(validators[i]->isValidatorsResponsibility(ch)) return validators[i];
    }
    return nullptr;
}

float EvaluatorBase::underAttackSubBonus(int ind, QString board, QString lastStep, float valSoFar)
{
    if(lastStep.contains('x')){
        QStringList stepDissasembled = lastStep.split('x');
        int to = stepDissasembled[1].toInt();

        if(ind == to){ // Chained attack maybe possible
            ValidatorBase* val = findValidator(board[ind]);
            if(!val) return 0;
            QSet<QString> pSteps = val->getValidIndecies(ind, board);

            if(!pSteps.isEmpty()){
                QString fStep = *pSteps.cbegin();
                // If it can attack, it has to so the piece won't stay here to be taken
                if(fStep.contains('x')) return 0;

                // If it doesn't cotain 'x' it can only take normal steps from here
                // and because the previous step was to get here that means this piece
                // is going to stay here.
            }
        }
    }

    QList<QPair<int, int>> directions;
    directions.append(QPair<int, int>(-1, -1));
    directions.append(QPair<int, int>(1, -1));
    directions.append(QPair<int, int>(1, 1));
    directions.append(QPair<int, int>(-1, 1));

    int iX = ind % 8;
    int iY = ind / 8;

    for(int i = 0; i < directions.length(); i++){
        int dX = directions[i].first;
        int dY = directions[i].second;

        int indX = iX + dX;
        int indY = iY + dY;

        while(indX >= 0 && indY >= 0 &&
               indX < 8 && indY < 8){

            int index = 8 * indY + indX;
            if(index > 63 || index < 0) break;

            if(board[index] != 'x'){ // Found a square where there is a piece on surface
                if(isOppositeTeam(board[ind], board[index])){ // That piece is from the opposite team
                    //Check if it can hit our piece
                    ValidatorBase* val = findValidator(board[index]);
                    QSet<QString> steps = val->getValidIndecies(index, board);
                    for (auto i = steps.cbegin(), end = steps.cend(); i != end; ++i){
                        QString stepValue = *i;
                        QStringList stepDissasembled = stepValue.split('x');
                        if(stepDissasembled.length() == 1) continue;

                        int targetInd = getIndOfTarget(stepDissasembled[0].toInt(), stepDissasembled[1].toInt());
                        if(ind == targetInd) return -valSoFar;
                    }
                }
                break;
            }
            indX += dX;
            indY += dY;
        }
    }
    return 0;
}

int EvaluatorBase::getIndOfTarget(int from, int to)
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

bool EvaluatorBase::isOppositeTeam(QChar p1, QChar p2)
{
    return (p1.isUpper() && p2.isLower()) || (p1.isLower() && p2.isUpper());
}

bool EvaluatorBase::isPawn(QChar p)
{
    return p == 'p' || p == 'P';
}

bool EvaluatorBase::isDame(QChar p)
{
    return p == 'd' || p == 'D';
}
