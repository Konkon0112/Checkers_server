#ifndef VALIDATORBASE_H
#define VALIDATORBASE_H

#include <QObject>
#include <QSet>
#include <QList>
#include <QPair>

class ValidatorBase : public QObject
{
    Q_OBJECT
public:
    explicit ValidatorBase(QObject *parent = nullptr);
    bool isValidatorsResponsibility(QChar c);
    bool isValidStep(QString step, QString board);
    virtual QSet<QString> getValidIndecies(int startInd, QString board) = 0;

private:
    int getIndFromCoordinates(QPair<int, int> coord);
    void getSteps(int sRow, int sCol, int dX, int dY, int maxIteration, QString board, QSet<QString> &validContainer);
    void checkIfPieceTakeable(int tCol, int tRow, int dx, int dY, int sInd, QString board, QSet<QString> &validContainer);

    bool isSameTeam(QChar c1, QChar c2);
    bool coordsAreOutsideTheBoard(int x, int y);
protected:
    QPair<QChar, QChar> validationChars;

    QSet<QString> getValidStepIndecies(int startingInd, QList<QPair<int, int>> dirPair, int maxIteration, QString board);

signals:
};

#endif // VALIDATORBASE_H
