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
        QStringList seperatedPacket = packetStr.split(ptKeeper->getPacketSeparator());
        emit stepInitiatedSignal(seperatedPacket[1]);
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

bool HumanParticipant::usingThisSocket(QTcpSocket* soc)
{
    return soc == socket;
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

void HumanParticipant::gameStartedSlot(Participant::ParticipantSideEnum nextOnTurn, QString stepsSoFar)
{
    QString packetType = ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::GAME_STARTED);
    QString nextColor = ptKeeper->enumToStringPieceColor(nextOnTurn);
    QString playerColor = ptKeeper->enumToStringPieceColor(getPSide());

    QByteArray message;
    message.append(packetType.toUtf8());
    message.append(ptKeeper->getPacketSeparator());
    message.append(nextColor.toUtf8());
    message.append(ptKeeper->getPacketSeparator());
    message.append(playerColor.toUtf8());
    message.append(ptKeeper->getPacketSeparator());
    message.append(stepsSoFar.toUtf8());

    socket->write(message);
}

void HumanParticipant::stepHappenedSlot(QString step)
{
    QString packetType = ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::STEP_HAPPENED);

    QByteArray message;
    message.append(packetType.toUtf8());
    message.append(ptKeeper->getPacketSeparator());
    message.append(step.toUtf8());
    socket->write(message);
}

void HumanParticipant::undoApprovedSlot()
{

}

void HumanParticipant::gameOverSlot(Participant::ParticipantSideEnum winner)
{

}

void HumanParticipant::turnChangedSlot(Participant::ParticipantSideEnum nextOnTurn)
{
    QString packetType = ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::TURN_CHANGED);
    QString nextColor = ptKeeper->enumToStringPieceColor(nextOnTurn);

    QByteArray message;
    message.append(packetType.toUtf8());
    message.append(ptKeeper->getPacketSeparator());
    message.append(nextColor.toUtf8());

    socket->write(message);
}
