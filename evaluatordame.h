#ifndef EVALUATORDAME_H
#define EVALUATORDAME_H

#include <QObject>

#include "evaluatorbase.h"
#include "validatordame.h"

class EvaluatorDame : public EvaluatorBase
{
    Q_OBJECT
public:
    explicit EvaluatorDame(QObject *parent = nullptr);

signals:

    // EvaluatorBase interface
public:
    float evaluatePiece(int ind, QString board, QString lastStep);

private:
    float addEdgeOfBoardBonus(int ind);
    float addNumOfTargetBonus(int ind, QString board, QString lastStep);
    int getIndOfTarget(int from, int to);

    ValidatorDame* dameValidator;
};

#endif // EVALUATORDAME_H
