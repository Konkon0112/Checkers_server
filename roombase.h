#ifndef ROOMBASE_H
#define ROOMBASE_H

#include <QObject>
#include <QTcpSocket>

class RoomBase : public QObject
{
    Q_OBJECT
public:
    enum RoomState {
        ACTIVE,
        WAITING_FOR_PLAYERS,
    };
    explicit RoomBase(QObject *parent = nullptr);

    void join(QTcpSocket* socket);
signals:
};

#endif // ROOMBASE_H
