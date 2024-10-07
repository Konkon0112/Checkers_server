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

HumanParticipant::~HumanParticipant()
{
    disconnect(socket, SIGNAL(readyRead()), this, SLOT(handlingReadyReadSlot()));
}

void HumanParticipant::handleIncommingPacket(QString packetStr, QString packetType)
{
    if(packetType ==
        ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::INITIATE_STEP)){
        qInfo() << socket << "INITIATE STEP";
    } else if (packetType ==
               ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::UNDO_STEP_INITIATED)) {
        qInfo() << socket << "INITIATE UNDO";
    } else if (packetType ==
               ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::APPROVE_UNDO)) {
        qInfo() << socket << "APPROVE UNDO";
    } else if (packetType ==
               ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::QUIT_GAME)) {
        qInfo() << socket << "QUIT GAME";
        emit playerQuitSignal();
    }
}

QTcpSocket *HumanParticipant::getSocket() const
{
    return socket;
}


void HumanParticipant::handlingReadyReadSlot()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if(!socket)return;

    QByteArray data = socket->readAll();
    qInfo() << "Data in participant" << data;
    QString dataTypeStr = ptKeeper->shouldParticipantHandle(data);
    if(dataTypeStr == ""){
        qInfo() << "Wrong packet: " << data;
        return;
    }
    handleIncommingPacket(QString(data), dataTypeStr);
}

void HumanParticipant::gameStartedSlot()
{

}

void HumanParticipant::stepHappenedSlot(QString step)
{

}

void HumanParticipant::undoApprovedSlot()
{

}

void HumanParticipant::gameOverSlot(Participant::ParticipantSideEnum winner)
{

}

void HumanParticipant::turnChangedSignal(ParticipantSideEnum nextOnTurn)
{

}

void HumanParticipant::turnChangedSlot()
{

}
