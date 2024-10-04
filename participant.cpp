#include "participant.h"

Participant::Participant(QTcpSocket* s,
                         ParticipantTypeEnum pT,
                         ParticipantSideEnum pS,
                         QObject *parent)
    : QObject{parent}
{
    socket = s;
    pType = pT;
    pSide = pS;
    ptKeeper = new PacketTypeKeeperService(this);
}

void Participant::handlingReadyReadSlot()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if(!socket)return;

    QByteArray data = socket->readAll();
    qInfo() << "Received data:" << data;
}


