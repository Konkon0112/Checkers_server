#ifndef EVALUATORPAWN_H
#define EVALUATORPAWN_H

#include <QObject>

#include "evaluatorbase.h"
#include "validatorpawn.h"

class EvaluatorPawn : public EvaluatorBase
{
    Q_OBJECT
public:
    explicit EvaluatorPawn(QObject *parent = nullptr);
    float evaluatePiece(int ind, QString board, QString lastStep) override;

private:
    float subAttackDanger(int ind, QString board, QString lastStep);

    ValidatorPawn* pawnValidator;

signals:
};

#endif // EVALUATORPAWN_H
