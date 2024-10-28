#ifndef EVALUATORBASE_H
#define EVALUATORBASE_H

#include <QObject>
#include <QChar>
#include <QList>
#include "validatorbase.h"

class EvaluatorBase : public QObject
{
    Q_OBJECT
public:
    explicit EvaluatorBase(QChar lightCh, QChar darkCh, float baseV, int validatorInd, QObject *parent = nullptr);

    /**
    * @brief User should check if the piece is this evaluator's responsibility.
    *
    * @return The evaluation value of the board, if the piece at the given index is not it's responsibility returns 0.
    */
    virtual float evaluatePiece(int ind, QString board, QString lastStep) = 0;
    bool isEvaluatorResponsibility(QChar ch);
signals:

protected:
    ValidatorBase* findValidator(QChar ch);
    float underAttackSubBonus(int ind, QString board, QString lastStep);
    int getIndOfTarget(int from, int to);
    bool isOppositeTeam(QChar p1, QChar p2);
    bool isPawn(QChar p);
    bool isDame(QChar p);

    float baseVal;
    int valIndex;

    QList<ValidatorBase*> validators;
private:
    QChar lightChar;
    QChar darkChar;
};

#endif // EVALUATORBASE_H
