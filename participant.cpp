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
    connect(s, SIGNAL(readyRead()), this, SLOT(handlingReadyReadSlot()));
}

bool Participant::isPlayerType(ParticipantTypeEnum pT)
{
    return pT == pType;
}

void Participant::handlingReadyReadSlot()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if(!socket)return;

    QByteArray data = socket->readAll();
    qInfo() << "Received data:" << data;
    QString dataTypeStr = ptKeeper->shouldParticipantHandle(data);
    if(dataTypeStr == ""){
        qInfo() << "Wrong packet: " << data;
        return;
    }
    qInfo() << "Good packet:" << data;
}


