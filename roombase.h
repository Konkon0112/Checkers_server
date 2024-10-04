#ifndef ROOMBASE_H
#define ROOMBASE_H

#include <QObject>
#include <QList>
#include <QTcpSocket>

#include "participant.h"

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

protected:
    RoomType roomType;
    QList<Participant*> pList;
    RoomState roomState;

private:
    int countPlayersInRoom();

private slots:
    void readyRead();

signals:

    void gameOver(QList<QTcpSocket*> pSocketList);
};

#endif // ROOMBASE_H
