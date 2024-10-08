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
    disconnect(socket, SIGNAL(disconnected()),this,SLOT(disconnected()));
    disconnect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    socket->deleteLater();
    //TODO: check if there is game related to player
}

void Server::readyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if(!socket)return;

    QByteArray data = socket->readAll();
    QString dataAsString(data);
    QStringList dataList = dataAsString.split('\n');

    for(int i = 0; i < dataList.length(); i++){
        if(dataList.at(i) == "") return;
        QString packetType = ptKeeper->shouldServerHandle(QString(data));

        if(packetType == "")return;
        handleIncommingPacket(dataList.at(i), socket);
    }
}

void Server::playerQuitGameSlot(QTcpSocket *socket)
{
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
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
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket,SIGNAL(readyRead()),this, SLOT(readyRead()));
    qInfo() << "Connection received";
}

void Server::handleIncommingPacket(QString packetStr, QTcpSocket* socket)
{
    QString packetType = ptKeeper->shouldServerHandle(packetStr);
    if(packetType == "") return;
    if(packetType ==
        ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::JOIN_NEW_SINGLE_GAME)){
        handleJoinNewSinglePlayer(packetStr, socket);
        qInfo() << socket << "Joined new single player";
    } else if (packetType ==
               ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::CONTINUE_SINGLE_GAME)) {
        handleContinueSinglePlayer(packetStr, socket);
        qInfo() << socket << "Continued single player";
    } else if (packetType ==
               ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::JOIN_MULTI_GAME)) {
        joinMultiPlayer(packetStr, socket); //TODO: delete packet string if nothing added to it in the end
        qInfo() << socket << "Joined multiplayer";
    }

    // Only join packets allowed here -> need to disconnect
    disconnect(socket,SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void Server::handleJoinNewSinglePlayer(QString data, QTcpSocket* socket)
{
    // [packet type]#[piece color the player is using]
    QStringList dataList = data.split('#');

    Participant::ParticipantSideEnum robotSideEnum =
        dataList[1] == ptKeeper->enumToStringPieceColor(Participant::ParticipantSideEnum::DARK)?
            Participant::ParticipantSideEnum::LIGHT: Participant::ParticipantSideEnum::DARK;

    HumanVsRobotRoom* newRoom = new HumanVsRobotRoom(robotSideEnum, this);
    connect(newRoom, SIGNAL(playerQuitGameSignal(QTcpSocket*)), this, SLOT(playerQuitGameSlot(QTcpSocket*)));
    newRoom->join(socket);
    rList.append(newRoom);
}

void Server::handleContinueSinglePlayer(QString data, QTcpSocket* socket)
{
    // [packet type]#[piece color the player is using]#[color on turn]#[steps so far joined]
    QStringList dataList = data.split('#');
    Participant::ParticipantSideEnum robotSideEnum =
        dataList[1] == ptKeeper->enumToStringPieceColor(Participant::ParticipantSideEnum::DARK)?
            Participant::ParticipantSideEnum::LIGHT: Participant::ParticipantSideEnum::DARK;

    Participant::ParticipantSideEnum colorOnTurn = ptKeeper->stringToEnumPieceColor(dataList[1]);
    HumanVsRobotRoom* newRoom = new HumanVsRobotRoom(robotSideEnum, this);
    newRoom->setUpContinuedGame(colorOnTurn, dataList[2]);

    connect(newRoom, SIGNAL(playerQuitGameSignal(QTcpSocket*)), this, SLOT(playerQuitGameSlot(QTcpSocket*)));
    newRoom->join(socket);
    rList.append(newRoom);
}

void Server::joinMultiPlayer(QString data, QTcpSocket* socket)
{
    // [packet type]
    HumanVsHumanRoom* newRoom = nullptr;
    for(int i = 0; i < rList.length(); i++){
        if(rList.at(i)->getRoomType() == RoomBase::RoomType::HUMAN_VS_HUMAN){
            newRoom = qobject_cast<HumanVsHumanRoom*>(rList.at(i));
            break;
        }
    }
    if(newRoom == nullptr){
        newRoom = new HumanVsHumanRoom(this);
        rList.append(newRoom);
    }
    connect(newRoom, SIGNAL(playerQuitGameSignal(QTcpSocket*)), this, SLOT(playerQuitGameSlot(QTcpSocket*)));
    newRoom->join(socket);
}
