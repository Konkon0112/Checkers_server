#ifndef ROBOTSTEPCALCULATOR_H
#define ROBOTSTEPCALCULATOR_H

#include <QObject>
#include <QRunnable>
#include <QString>
#include <QList>
#include <QSet>

#include "participant.h"
#include "validatorbase.h"
#include "positionevaluator.h"


class RobotStepCalculator : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit RobotStepCalculator(QString board, Participant::ParticipantSideEnum robotS, QObject *parent = nullptr);

signals:
    void stepCalculated(QString step);
public:
    void run();

private:

    QString makeMoveOnBoard(QString board, QString step);
    int minimax(QString board, int depth, int& alpha, int& beta, bool isMaxPlayer);
    int maxVal(int x, int y);
    int minVal(int x, int y);

    bool isMaximizingPlayer;
    QString board;
    const int searchDepth = 4;
    PositionEvaluator* evaluator;
};

#endif // ROBOTSTEPCALCULATOR_H
