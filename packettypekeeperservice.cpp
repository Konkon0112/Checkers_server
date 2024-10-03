#include "packettypekeeperservice.h"

PacketTypeKeeperService::PacketTypeKeeperService(QObject *parent)
    : QObject{parent}
{
    packetMap.insert(PacketTypeEnum::JOIN_NEW_SINGLE_GAME, "JOIN_NEW_SINGLE_GAME");
    packetMap.insert(PacketTypeEnum::JOIN_MULTI_GAME, "JOIN_MULTI_GAME");
    packetMap.insert(PacketTypeEnum::QUIT_GAME, "QUIT_GAME");
    packetMap.insert(PacketTypeEnum::GAME_OVER, "GAME_OVER");
    packetMap.insert(PacketTypeEnum::INITIATE_STEP, "INITIATE_STEP");
    packetMap.insert(PacketTypeEnum::STEP_HAPPENED, "STEP_HAPPENED");
    packetMap.insert(PacketTypeEnum::UNDO_STEP_INITIATED, "UNDO_STEP_INITIATED");
    packetMap.insert(PacketTypeEnum::UNDO_STEP_APPROVED, "UNDO_STEP_APPROVED");
}

QString PacketTypeKeeperService::enumToString(PacketTypeEnum pT)
{
    return packetMap[pT];
}

PacketTypeKeeperService::PacketTypeEnum PacketTypeKeeperService::stringToEnum(QString pTInString)
{
    for (auto it = packetMap.constBegin(); it != packetMap.constEnd(); ++it) {
        if(it.value() == pTInString) return it.key();
    }
}
