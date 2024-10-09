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

    void dealWithDisconnectedParticipant(QTcpSocket* socket);

protected:
    int countPlayersInRoom();

    RoomType roomType;
    QList<Participant*> pList;
    RoomState roomState;
    GameModel* gameModel;    
private:

protected slots:
    virtual void playerQuitSlot() = 0;  // Received from player

private slots:
    void stepInitiatedSlot(QString step);
    void undoInitiatedSlot();
    void approveUndoSlot();
    void stepHappenedSlot(QString step); // Received from game model
    void turnChangedSlot(Participant::ParticipantSideEnum nextOnTurn);

signals:
    void playerQuitGameSignal(QTcpSocket* socket); // Room sends to server for it to reconnect to readyRead
    void gameStartedSignal(Participant::ParticipantSideEnum colorOnTurn, QString stepsSoFar); // Sent to all participant
    void undoApprovedSignal(); //
    void stepHappenedSignal(QString step); // Sent to players
    void gameOverSignal(Participant::ParticipantSideEnum winner); // Sent to players
    void turnChangedSignal(Participant::ParticipantSideEnum nextOnTurn); // Sent to players
};

#endif // ROOMBASE_H
