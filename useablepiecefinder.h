#ifndef USEABLEPIECEFINDER_H
#define USEABLEPIECEFINDER_H

#include <QObject>
#include <QList>
#include <QSet>

#include "validatorbase.h"
#include "participant.h"

class UseablePieceFinder : public QObject
{
    Q_OBJECT
public:
    explicit UseablePieceFinder(QObject *parent = nullptr);

    /**
    * @brief Find the indecies of all the useable piece from the given color.
    */
    QSet<int> findUseablePieceIndecies(QString board, Participant::ParticipantSideEnum userPiece);

private:
    QList<ValidatorBase*> validators;
    ValidatorBase *findResponsibleValidator(QChar ch);
    bool containsStepThatCanTake(QSet<QString> validSteps);

signals:
};

#endif // USEABLEPIECEFINDER_H
