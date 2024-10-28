#include "evaluatorpawn.h"

EvaluatorPawn::EvaluatorPawn(QObject *parent)
    : EvaluatorBase{'P', 'p', 1, 0, parent}
{}

float EvaluatorPawn::evaluatePiece(int ind, QString board, QString lastStep)
{
    if(board[ind] == 'x') return 0;
    float base = baseVal;
    bool isDark = board[ind].isLower();

    int y = ind / 8;

    float sRowBonus = 0;
    float tRowBonus = 0;

    // A pawn if made it to the last line it becomes a dame
    if(isDark){
        if(y == 5) sRowBonus += 0.5;
        if(y == 6) tRowBonus += 1;
    } else {
        if(y == 2) sRowBonus += 0.5;
        if(y == 1) tRowBonus += 1;
    }

    float beingAttackedDanger = underAttackSubBonus(ind, board, lastStep);

    base = base + sRowBonus + tRowBonus + beingAttackedDanger;

    return isDark? -base : base;
}
