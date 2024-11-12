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
    explicit RobotStepCalculator(QString board, Participant::ParticipantSideEnum robotS, QString lStep = "", QObject *parent = nullptr);

signals:
    /**
    * @brief Emited when the next step calculation is complete.
    */
    void stepCalculated(QString step);
public:
    void run();

private:

    QString makeMoveOnBoard(QString board, QString step);
    QPair<float, QString> minimax(QString board, QString lastStep, int depth, QPair<float, QString>& alpha, QPair<float, QString>& beta, bool isMaxPlayer);
    QPair<float, QString> maxVal(QPair<float, QString> x, QPair<float, QString> y);
    QPair<float, QString> minVal(QPair<float, QString> x, QPair<float, QString> y);
    ValidatorBase* getValidator(QChar square);
    QList<QString> getAllPossibleSteps(QString board, bool isMaxPlayer, QString lastStep);
    bool isChainedPossible(QString lastStep, QString board);

    bool isMaximizingPlayer;
    QString board;
    const int searchDepth = 4;
    PositionEvaluator* evaluator;
    QList<ValidatorBase*> validators;
    QString lastStep;
};

#endif // ROBOTSTEPCALCULATOR_H
