#ifndef ROBOTSTEPCALCULATOR_H
#define ROBOTSTEPCALCULATOR_H

#include <QObject>
#include <QRunnable>
#include <QString>
#include <QList>
#include <QSet>
#include <QPair>

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
    QPair<float, QString> minimax(QString board, QString lastStep, int depth, QPair<float, QString>& alpha, QPair<float, QString>& beta, bool isMaxPlayer);
    QPair<float, QString> maxVal(QPair<float, QString> x, QPair<float, QString> y);
    QPair<float, QString> minVal(QPair<float, QString> x, QPair<float, QString> y);
    ValidatorBase* getValidator(QChar square);
    QList<QString> getAllPossibleSteps(QString board, bool isMaxPlayer);
    bool isChainedPossible(QString lastStep, QString board);

    bool isMaximizingPlayer;
    QString board;
    const int searchDepth = 3;
    PositionEvaluator* evaluator;
    QList<ValidatorBase*> validators;
};

#endif // ROBOTSTEPCALCULATOR_H
