#include "humanparticipant.h"

HumanParticipant::HumanParticipant(QTcpSocket* s,
                                   ParticipantTypeEnum pT,
                                   ParticipantSideEnum pS,
                                   QObject *parent)
    : Participant{pT, pS, parent}
{
    socket = s;
    connect(socket, SIGNAL(readyRead()), this, SLOT(handlingReadyReadSlot()));
}


void HumanParticipant::handlingReadyReadSlot()
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

void HumanParticipant::gameStartedSlot()
{

}

void HumanParticipant::stepHappenedSlot()
{

}

void HumanParticipant::gameOverSlot()
{

}

void HumanParticipant::turnChangedSlot()
{

}
