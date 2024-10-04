#include <QDebug>
#include "server.h"
#include "humanvsrobotroom.h"
#include "humanvshumanroom.h"
#include "participant.h"

Server::Server(QObject *parent)
    : QTcpServer{parent}
{
    ptKeeper = new PacketTypeKeeperService(this);

    HumanVsHumanRoom* newRoom = new HumanVsHumanRoom(this);
    rList.append(newRoom);
}

void Server::startListening()
{
    if(!this->listen(QHostAddress::Any, port))
    {
        qInfo() << "Error" << this->errorString();
        return;
    }
    qInfo() << "Server running";
}

void Server::closeServer()
{
    foreach (QTcpSocket *socket, sList)
    {
        socket->close();
    }
    qDeleteAll(sList);
    sList.clear();


    QTcpServer::close();
}

void Server::disconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if(!socket)return;
    qInfo() << "Socket disconnected" << socket;

    sList.removeAll(socket);
    disconnect(socket,&QTcpSocket::disconnected,this,&Server::disconnected);
    disconnect(socket,&QTcpSocket::readyRead,this,&Server::readyRead);
    socket->deleteLater();
}

void Server::readyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if(!socket)return;

    QByteArray data = socket->readAll();
    QString packetType = ptKeeper->shouldServerHandle(QString(data));

    if(packetType ==
        ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::JOIN_NEW_SINGLE_GAME)){
        qInfo() << data << "NEW S";

        QString dataStr(data);
        Participant::ParticipantSideEnum pSideEnum =
            dataStr.contains(ptKeeper->enumToStringPieceColor(Participant::ParticipantSideEnum::DARK))?
            Participant::ParticipantSideEnum::DARK: Participant::ParticipantSideEnum::LIGHT;

        HumanVsRobotRoom* newRoom = new HumanVsRobotRoom(pSideEnum, this);
        newRoom->join(socket);
        rList.append(newRoom);

    } else if (packetType ==
               ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::CONTINUE_SINGLE_GAME)) {
        qInfo() << data << "CONTINUE S";
    } else if (packetType ==
               ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::JOIN_MULTI_GAME)) {
        qInfo() << data << "MULTI";
    }else {
        qInfo() << "Not server's responsibility!" << data;
    }
}

void Server::incomingConnection(qintptr handle)
{
    QTcpSocket *socket = new QTcpSocket();
    socket->setSocketDescriptor(handle);

    if(!socket->waitForConnected(3000))
    {
        delete socket;
        return;
    }

    sList.append(socket);
    connect(socket,&QTcpSocket::disconnected,this,&Server::disconnected);
    connect(socket,&QTcpSocket::readyRead,this,&Server::readyRead);
    qInfo() << "Connection received";
}

void Server::handleIncommingPacket(QString packetStr)
{
    if(packetStr ==
        ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::JOIN_NEW_SINGLE_GAME)){

    } else if(packetStr ==
               ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::CONTINUE_SINGLE_GAME)){

    } else if(packetStr ==
               ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::JOIN_MULTI_GAME)){

    }
}
