#ifndef POSITIONEVALUATOR_H
#define POSITIONEVALUATOR_H

#include <QObject>
#include <QString>
#include <QList>

#include "board.h"
#include "evaluatorbase.h"

class PositionEvaluator : public QObject
{
    Q_OBJECT
public:
    explicit PositionEvaluator(QObject *parent = nullptr);

    /**
    * @brief Evaluate the board state using the lastStep.
    *
    * @return The evaluation value.
    */
    float evaluatePosition(QString board, QString lastStep);

private:
    EvaluatorBase *findEvaluator(QChar ch);

    Board* board;
    QList<EvaluatorBase*> evaluators;
};

#endif // POSITIONEVALUATOR_H
