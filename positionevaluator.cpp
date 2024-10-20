#include "positionevaluator.h"
#include "evaluatordame.h"
#include "evaluatorpawn.h"

PositionEvaluator::PositionEvaluator(QObject *parent)
    : QObject{parent}
{
    board = new Board(this);

    evaluators.append(new EvaluatorDame(this));
    evaluators.append(new EvaluatorPawn(this));
}

float PositionEvaluator::evaluatePosition(QString board, QString lastStep)
{
    float result = 0;
    for(int i = 0; i < board.length(); i++){
        EvaluatorBase* evaluator = findEvaluator(board[i]);
        if(!evaluator) continue;

        result += evaluator->evaluatePiece(i, board, lastStep);
    }
    return result;
}

EvaluatorBase *PositionEvaluator::findEvaluator(QChar ch)
{
    EvaluatorBase* res = nullptr;
    for(int i = 0; i < evaluators.length(); i++){
        if(evaluators[i]->isEvaluatorResponsibility(ch)) return evaluators[i];
    }
    return res;
}
