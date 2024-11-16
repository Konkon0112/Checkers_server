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

    void startGame();
    /**
    * @brief Restart the board and set color on turn to LIGHT.
    */
    void restartGame();

    /**
    * @brief Validate and execute step, if the given step is not valid throw CustomException.
    */
    void executeStep(QString step);

    /**
    * @brief Execute step.
    * @warning Without step validation.
    */
    void passStepOnSetUp(QString step);

    /**
    * @brief Undo all the steps that happened until the last step of the given color.
    */
    void undoStep(Participant::ParticipantSideEnum playerWhoInitiated);
    /**
    * @return The color of the player's side that should make the next move.
    */
    Participant::ParticipantSideEnum getColorOnTurn() const;

    /**
    * @return All the steps that happened from the start of the game joined with ';'.
    */
    QString getJoinedStepStr();
    void setColorOnTurn(Participant::ParticipantSideEnum newColorOnTurn);
    /**
    * @brief Set up the game state based on the given steps and the next color.
    */
    void setUpContinuedGame(QString stepsSoFar, Participant::ParticipantSideEnum nextColor);

signals:
    void stepHappenedSignal(QString step, Participant::ParticipantSideEnum newTurnColor);
    void gameOverSignal(Participant::ParticipantSideEnum winner);
    void undoHappenedSignal(QString newStepsSoFar, Participant::ParticipantSideEnum newColorOnTurn);

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
