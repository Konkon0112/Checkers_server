#include "evaluatorpawn.h"

EvaluatorPawn::EvaluatorPawn(QObject *parent)
    : EvaluatorBase{'P', 'p', 1, 0, parent}
{}

float EvaluatorPawn::evaluatePiece(int ind, QString board, QString lastStep)
{
    if(board[ind] == 'x') return 0;
    float res = baseVal;
    bool isDark = board[ind].isLower();

    int y = ind / 8;

    // A pawn if make it to the last line it becomes a dame
    if(isDark){
        if(y == 5) res += 0.5;
        if(y == 6) res += 1;
    } else {
        if(y == 2) res += 0.5;
        if(y == 1) res += 1;
    }

    res += underAttackSubBonus(ind, board, lastStep, res);

    return isDark? -res : res;
}
