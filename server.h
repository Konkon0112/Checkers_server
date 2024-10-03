#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

#include "packettypekeeperservice.h"

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


protected:
    void incomingConnection(qintptr handle) override;

signals:

private:
    QList<QTcpSocket*> m_list;
    PacketTypeKeeperService* ptKeeper;
    QChar packetSeperator = "#";
    int port = 3000;
};

#endif // SERVER_H
