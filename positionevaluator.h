#ifndef POSITIONEVALUATOR_H
#define POSITIONEVALUATOR_H

#include <QObject>
#include <QString>
#include "board.h"

class PositionEvaluator : public QObject
{
    Q_OBJECT
public:
    explicit PositionEvaluator(QObject *parent = nullptr);

signals:

private:
    int evaluatePosition(QString board);

    Board* board;
};

#endif // POSITIONEVALUATOR_H
