#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

#include "packettypekeeperservice.h"
#include "roombase.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void startListening();
    void closeServer();

public slots:

    void disconnected();
    void readyRead();

private slots:
    /**
    * @brief Take on the responsibility to read the messages comming from the client again.
    */
    void playerQuitGameSlot(QTcpSocket* socket);
    void removeRoomFromListSlot();

protected:
    void incomingConnection(qintptr handle) override;

signals:

private:
    void handleIncommingPacket(QString packetStr, QTcpSocket* socket);

    void handleJoinNewSinglePlayer(QString data, QTcpSocket* socket);
    void handleContinueSinglePlayer(QString data, QTcpSocket* socket);
    void joinMultiPlayer(QTcpSocket* socket);

    QList<QTcpSocket*> sList;
    QList<RoomBase*> rList;
    PacketTypeKeeperService* ptKeeper;
    QChar packetSeperator = '#';
    int port = 3000;
};

#endif // SERVER_H
