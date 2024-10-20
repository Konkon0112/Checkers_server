#ifndef EVALUATORBASE_H
#define EVALUATORBASE_H

#include <QObject>
#include <QChar>

class EvaluatorBase : public QObject
{
    Q_OBJECT
public:
    explicit EvaluatorBase(QChar lightCh, QChar darkCh, float baseV, QObject *parent = nullptr);

    /**
    * @brief User should check if the piece is this evaluator's responsibility.
    *
    * @return The evaluation value of the board, if the piece at the given index is not it's responsibility returns 0.
    */
    virtual float evaluatePiece(int ind, QString board, QString lastStep) = 0;
    bool isEvaluatorResponsibility(QChar ch);
signals:

protected:
    float baseVal;

private:
    QChar lightChar;
    QChar darkChar;
};

#endif // EVALUATORBASE_H
