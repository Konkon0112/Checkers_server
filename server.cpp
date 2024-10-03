#include "server.h"
#include <QDebug>

Server::Server(QObject *parent)
    : QTcpServer{parent}
{}

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
    foreach (QTcpSocket *socket, m_list)
    {
        socket->close();
    }
    qDeleteAll(m_list);
    m_list.clear();


    QTcpServer::close();
}

void Server::disconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if(!socket)return;
    qInfo() << "Socket disconnected" << socket;

    m_list.removeAll(socket);
    disconnect(socket,&QTcpSocket::disconnected,this,&Server::disconnected);
    disconnect(socket,&QTcpSocket::readyRead,this,&Server::readyRead);
    socket->deleteLater();
}

void Server::readyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if(!socket)return;

    QByteArray data = socket->readAll();
    foreach(QTcpSocket *socket, m_list)
    {
        socket->write(data);
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

    m_list.append(socket);
    connect(socket,&QTcpSocket::disconnected,this,&Server::disconnected);
    connect(socket,&QTcpSocket::readyRead,this,&Server::readyRead);
    qInfo() << "Connection received";
}
