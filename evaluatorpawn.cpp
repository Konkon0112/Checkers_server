#include "evaluatorpawn.h"

EvaluatorPawn::EvaluatorPawn(QObject *parent)
    : EvaluatorBase{'P', 'p', 1, parent}
{
    pawnValidator = new ValidatorPawn(this);
}

float EvaluatorPawn::evaluatePiece(int ind, QString board, QString lastStep)
{
    float base = baseVal;
    bool isDark = board[ind].isLower();

    int y = ind / 8;

    float sRowBonus = 0;
    float tRowBonus = 0;

    if(isDark){
        if(y == 5) sRowBonus += 0.5;
        if(y == 6) tRowBonus += 1;
    } else {
        if(y == 2) sRowBonus += 0.5;
        if(y == 1) tRowBonus += 1;
    }

    float beingAttackedDanger = subAttackDanger(ind, board, lastStep);

    base = base + sRowBonus + tRowBonus + beingAttackedDanger;

    return isDark? -base : base;
}

float EvaluatorPawn::subAttackDanger(int ind, QString board, QString lastStep)
{
    QStringList sl = lastStep.split('-');
    if(sl.length() == 1){
        sl = lastStep.split('x');
    }
    int to = sl[1].toInt();
    QSet<QString> pSteps = pawnValidator->getValidIndecies(ind, board);
    if (pSteps.isEmpty()){
        // TODO: still can be taken by opponent pawn.
        return 0;
    }

    QString firstStepVal = *pSteps.cbegin();

    if(firstStepVal.contains('x')){ // It can take AND can be taken
        if(ind == to){ // Last step was made with this pawn -> chained attack possible
            return 1;
    }
    return 0;
}
