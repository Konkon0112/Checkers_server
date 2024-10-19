#include "packettypekeeperservice.h"
#include "participant.h"

PacketTypeKeeperService::PacketTypeKeeperService(QObject *parent)
    : QObject{parent}
{
    packetMap.insert(PacketTypeEnum::JOIN_NEW_SINGLE_GAME, "JOIN_NEW_SINGLE_GAME");
    packetMap.insert(PacketTypeEnum::CONTINUE_SINGLE_GAME, "CONTINUE_SINGLE_GAME");
    packetMap.insert(PacketTypeEnum::JOIN_MULTI_GAME, "JOIN_MULTI_GAME");
    packetMap.insert(PacketTypeEnum::QUIT_GAME, "QUIT_GAME");
    packetMap.insert(PacketTypeEnum::GAME_STARTED, "GAME_STARTED");
    packetMap.insert(PacketTypeEnum::GAME_OVER, "GAME_OVER");
    packetMap.insert(PacketTypeEnum::INITIATE_STEP, "INITIATE_STEP");
    packetMap.insert(PacketTypeEnum::STEP_HAPPENED, "STEP_HAPPENED");
    packetMap.insert(PacketTypeEnum::UNDO_STEP_INITIATED, "UNDO_STEP_INITIATED");
    packetMap.insert(PacketTypeEnum::UNDO_STEP_APPROVED, "UNDO_STEP_APPROVED");
    packetMap.insert(PacketTypeEnum::APPROVE_UNDO, "APPROVE_UNDO");
    packetMap.insert(PacketTypeEnum::REJECT_UNDO, "REJECT_UNDO");
    packetMap.insert(PacketTypeEnum::NOTIFICATION, "NOTIFICATION");

    //Collect the packets the server object should handle
    serverList.append(PacketTypeEnum::JOIN_NEW_SINGLE_GAME);
    serverList.append(PacketTypeEnum::CONTINUE_SINGLE_GAME);
    serverList.append(PacketTypeEnum::JOIN_MULTI_GAME);

    //Collect the packets the participant should handle
    participantList.append(PacketTypeEnum::QUIT_GAME);
    participantList.append(PacketTypeEnum::INITIATE_STEP);
    participantList.append(PacketTypeEnum::UNDO_STEP_INITIATED);
    participantList.append(PacketTypeEnum::APPROVE_UNDO);
    participantList.append(PacketTypeEnum::REJECT_UNDO);

    pieceColorMap.insert(Participant::ParticipantSideEnum::DARK, "DARK");
    pieceColorMap.insert(Participant::ParticipantSideEnum::LIGHT, "LIGHT");

    toastTypeMap.insert(ToastTypeEnum::ERROR, "ERROR");
    toastTypeMap.insert(ToastTypeEnum::INFO, "INFO");
    toastTypeMap.insert(ToastTypeEnum::WARNING, "WARNING");
}

QString PacketTypeKeeperService::enumToStringPacketType(PacketTypeEnum pT)
{
    return packetMap[pT];
}

PacketTypeKeeperService::PacketTypeEnum PacketTypeKeeperService::stringToEnumPacketType(QString pTInString)
{
    for (auto it = packetMap.constBegin(); it != packetMap.constEnd(); ++it) {
        if(it.value() == pTInString) return it.key();
    }
    return PacketTypeKeeperService::PacketTypeEnum::UNKNOWN;
}

QString PacketTypeKeeperService::enumToStringPieceColor(Participant::ParticipantSideEnum pC)
{
    return pieceColorMap[pC];
}

Participant::ParticipantSideEnum PacketTypeKeeperService::stringToEnumPieceColor(QString pCInString)
{
    for (auto it = pieceColorMap.constBegin(); it != pieceColorMap.constEnd(); ++it) {
        if(it.value() == pCInString) return it.key();
    }
    return Participant::ParticipantSideEnum::NONE;
}

QString PacketTypeKeeperService::enumToStringToastType(ToastTypeEnum t)
{
    return toastTypeMap[t];
}

ToastTypeEnum PacketTypeKeeperService::stringToEnumToastType(QString tInString)
{
    for (auto it = toastTypeMap.constBegin(); it != toastTypeMap.constEnd(); ++it) {
        if(it.value() == tInString) return it.key();
    }
    return ToastTypeEnum::NONE;
}
/**
 * @brief Finds out if the received packet should be handled by the server.
 *
 * @return The corresponding enum in QString if it should not be handled by the server then returns "".
 */
QString PacketTypeKeeperService::shouldServerHandle(QString packet)
{
    for(int i = 0; i < serverList.size(); i++){
        QString enumInStr = enumToStringPacketType(serverList.at(i));
        if(packet.startsWith(enumInStr)) return enumInStr;
    }
    return "";
}

/**
 * @brief Finds out if the received packet should be handled by the participant object.
 *
 * @return The corresponding enum in QString if it should not be handled by the participant object then returns "".
 */
QString PacketTypeKeeperService::shouldParticipantHandle(QString packet)
{
    for(int i = 0; i < participantList.size(); i++){
        QString enumInStr = enumToStringPacketType(participantList.at(i));
        if(packet.startsWith(enumInStr)) return enumInStr;
    }
    return "";
}

QByteArray PacketTypeKeeperService::getPacketSeparator()
{
    return QString("#").toUtf8();
}
