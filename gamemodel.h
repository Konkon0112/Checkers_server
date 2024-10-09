#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include <QList>
#include <QStringList>

#include "board.h"
#include "packettypekeeperservice.h"
#include "participant.h"
#include "validatorbase.h"

class GameModel : public QObject
{
    Q_OBJECT
public:
    explicit GameModel(QObject *parent = nullptr);

    void resetGame();
    void passStepForward(QString step);
    void undoStep(Participant::ParticipantSideEnum playerWhoInitiated);
    Participant::ParticipantSideEnum getColorOnTurn() const;
    QString getJoinedStepStr();
    void setColorOnTurn(Participant::ParticipantSideEnum newColorOnTurn);

signals:
    void stepHappenedSignal(QString step);
    void turnChangedSignal(Participant::ParticipantSideEnum newTurnColor);
    void gameOverSignal(Participant::ParticipantSideEnum winner);

private:
    void checkIfGameOver(Participant::ParticipantSideEnum playerOnTurnSide);
    void completeTasksOnTurnChange(Participant::ParticipantSideEnum playerOnTurnSide);

    Participant::ParticipantSideEnum colorOnTurn;
    Board* board;
    QList<ValidatorBase*> validators;
    QStringList stepList;
};

#endif // GAMEMODEL_H
