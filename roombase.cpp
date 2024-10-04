#include "roombase.h"

RoomBase::RoomBase(RoomState rs, QObject *parent)
    : QObject{parent}
{
    roomState = rs;
}

void RoomBase::join(QTcpSocket *socket)
{

}

void RoomBase::readyRead()
{

}
