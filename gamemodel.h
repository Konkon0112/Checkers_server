#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include <QList>
#include <QPair>
#include <QStringList>

#include "board.h"
#include "packettypekeeperservice.h"
#include "participant.h"
#include "validatorbase.h"
#include "useablepiecefinder.h"

class GameModel : public QObject
{
    Q_OBJECT
public:
    explicit GameModel(QObject *parent = nullptr);

    enum class GameState {
        UNDER_STEUP,
        ACTIVE,
        FINISHED,
    };

    void restartGame();
    void passStepForward(QString step);
    void passStepOnSetUp(QString step);
    void undoStep(Participant::ParticipantSideEnum playerWhoInitiated);
    Participant::ParticipantSideEnum getColorOnTurn() const;
    QString getJoinedStepStr();
    void setColorOnTurn(Participant::ParticipantSideEnum newColorOnTurn);
    void setUpContinuedGame(QString stepsSoFar);

signals:
    void stepHappenedSignal(QString step);
    void turnChangedSignal(Participant::ParticipantSideEnum newTurnColor);
    void gameOverSignal(Participant::ParticipantSideEnum winner);
    void undoHappenedSignal(QString newStepsSoFar);

private:
    bool checkIfGameOver(Participant::ParticipantSideEnum playerOnTurnSide);
    void completeTasksOnTurnChange(Participant::ParticipantSideEnum playerOnTurnSide);
    void addStepToList(QString step);
    void updateUseablePieces(int chainInd = -1);

    UseablePieceFinder* useablePieceFinder;
    QSet<int> useablePieces;
    GameState state;
    Participant::ParticipantSideEnum colorOnTurn;
    Board* board;
    QList<ValidatorBase*> validators;
    QList<QPair<Participant::ParticipantSideEnum, QStringList>> stepList;
};

#endif // GAMEMODEL_H
