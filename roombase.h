#ifndef ROOMBASE_H
#define ROOMBASE_H

#include <QObject>
#include <QList>
#include <QTcpSocket>

#include "participant.h"
#include "gamemodel.h"

// Needed a base class for this, because I want to create the robot
// player in the constructor of the HumanVsRobotRoom class
class RoomBase : public QObject
{
    Q_OBJECT
public:
    enum RoomState {
        ACTIVE,
        WAITING_FOR_PLAYERS,
        FINISHED,
    };

    enum RoomType {
        HUMAN_VS_HUMAN,
        HUMAN_VS_ROBOT,
    };
    explicit RoomBase(RoomState rs, QObject *parent = nullptr);

    void join(QTcpSocket* socket);

    RoomState getRoomState() const;

    RoomType getRoomType() const;

protected:
    RoomType roomType;
    QList<Participant*> pList;
    RoomState roomState;
    GameModel* gameModel;

    Participant::ParticipantSideEnum playerOnTurn = Participant::ParticipantSideEnum::NONE;

private:
    int countPlayersInRoom();
    void startGame();

private slots:
    void stepInitiatedSlot(QString step);
    void playerQuitSlot();
    void undoInitiatedSlot();
    void approveUndoSlot();

signals:
    void gameStarted();
    void undoApprovedSignal();
    void stepHappenedSignal(QString step);
    void gameOver(Participant::ParticipantSideEnum winner);
    void turnChangedSignal(Participant::ParticipantSideEnum nextOnTurn);
};

#endif // ROOMBASE_H
