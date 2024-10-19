#include "positionevaluator.h"

PositionEvaluator::PositionEvaluator(QObject *parent)
    : QObject{parent}
{
    board = new Board(this);
}

int PositionEvaluator::evaluatePosition(QString board)
{

}
