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

void HumanParticipant::handleIncommingPacket(QString packetStr)
{
    qInfo() << "packet content:" << packetStr;
    QString packetType = ptKeeper->shouldParticipantHandle(packetStr);
    if(packetType ==
        ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::INITIATE_STEP)){
        qInfo() << socket << "INITIATE STEP";
        QStringList seperatedPacket = packetStr.split(ptKeeper->getPacketSeparator());
        emit stepInitiatedSignal(seperatedPacket[1]);

    } else if (packetType ==
               ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::UNDO_STEP_INITIATED)) {
        qInfo() << socket << "INITIATE UNDO";
        emit undoInitiatedSignal();

    } else if (packetType ==
               ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::APPROVE_UNDO)) {
        qInfo() << socket << "APPROVE UNDO";
        emit approveUndoSignal();

    } else if (packetType ==
               ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::REJECT_UNDO)) {
        qInfo() << socket << "REJECT UNDO";
        emit rejectUndoSignal();

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
    QString dataAsString(data);
    QStringList dataList = dataAsString.split('\n');

    for(int i = 0; i < dataList.length(); i++){
        if(dataList.at(i) == "") return;
        QString packetType = ptKeeper->shouldParticipantHandle(QString(data));

        if(packetType == "")return;
        handleIncommingPacket(QString(dataList.at(i)));
    }

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

    sendMessage(message);
}

void HumanParticipant::stepHappenedSlot(QString step, Participant::ParticipantSideEnum newTurnColor)
{
    QString packetType = ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::STEP_HAPPENED);
    QString turnString = ptKeeper->enumToStringPieceColor(newTurnColor);

    QByteArray message;
    message.append(packetType.toUtf8());
    message.append(ptKeeper->getPacketSeparator());
    message.append(step.toUtf8());
    message.append(ptKeeper->getPacketSeparator());
    message.append(turnString.toUtf8());

    sendMessage(message);
}

void HumanParticipant::undoNeedsApprovalSlot(Participant::ParticipantSideEnum approvingSideColor)
{
    if(pSide != approvingSideColor) return;
    QString packetType = ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::UNDO_STEP_INITIATED);

    QByteArray message;
    message.append(packetType.toUtf8());

    sendMessage(message);
}

void HumanParticipant::undoHappenedSlot(QString newStepsSoFar, Participant::ParticipantSideEnum nextC)
{
    QString packetType = ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::UNDO_STEP_APPROVED);
    QString turnString = ptKeeper->enumToStringPieceColor(nextC);

    QByteArray message;
    message.append(packetType.toUtf8());
    message.append(ptKeeper->getPacketSeparator());
    message.append(newStepsSoFar.toUtf8());
    message.append(ptKeeper->getPacketSeparator());
    message.append(turnString.toUtf8());

    sendMessage(message);
}

void HumanParticipant::gameOverSlot(Participant::ParticipantSideEnum winner)
{
    QString packetType = ptKeeper->enumToStringPacketType(PacketTypeKeeperService::PacketTypeEnum::GAME_OVER);
    QString w = ptKeeper->enumToStringPieceColor(winner);

    QByteArray message;
    message.append(packetType.toUtf8());
    message.append(ptKeeper->getPacketSeparator());
    message.append(w.toUtf8());

    sendMessage(message);
}

void HumanParticipant::sendMessage(QByteArray message)
{
    // We are using delimeter based protocol
    // Because of the changing packet sizes
    message.append('\n');
    socket->write(message);
    socket->flush();
}
