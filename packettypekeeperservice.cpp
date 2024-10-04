#include "packettypekeeperservice.h"

PacketTypeKeeperService::PacketTypeKeeperService(QObject *parent)
    : QObject{parent}
{
    packetMap.insert(PacketTypeEnum::JOIN_NEW_SINGLE_GAME, "JOIN_NEW_SINGLE_GAME");
    packetMap.insert(PacketTypeEnum::CONTINUE_SINGLE_GAME, "CONTINUE_SINGLE_GAME");
    packetMap.insert(PacketTypeEnum::JOIN_MULTI_GAME, "JOIN_MULTI_GAME");
    packetMap.insert(PacketTypeEnum::SEND_STEPS_SO_FAR, "SEND_STEPS_SO_FAR");
    packetMap.insert(PacketTypeEnum::QUIT_GAME, "QUIT_GAME");
    packetMap.insert(PacketTypeEnum::GAME_OVER, "GAME_OVER");
    packetMap.insert(PacketTypeEnum::INITIATE_STEP, "INITIATE_STEP");
    packetMap.insert(PacketTypeEnum::STEP_HAPPENED, "STEP_HAPPENED");
    packetMap.insert(PacketTypeEnum::UNDO_STEP_INITIATED, "UNDO_STEP_INITIATED");
    packetMap.insert(PacketTypeEnum::UNDO_STEP_APPROVED, "UNDO_STEP_APPROVED");

    //Collect the packets the server object should handle
    serverList.append(PacketTypeEnum::JOIN_NEW_SINGLE_GAME);
    serverList.append(PacketTypeEnum::CONTINUE_SINGLE_GAME);
    serverList.append(PacketTypeEnum::JOIN_MULTI_GAME);

    //Collect the packets the participant should handle
    participantList.append(PacketTypeEnum::SEND_STEPS_SO_FAR);
    participantList.append(PacketTypeEnum::QUIT_GAME);
    participantList.append(PacketTypeEnum::GAME_OVER);
    participantList.append(PacketTypeEnum::INITIATE_STEP);
    participantList.append(PacketTypeEnum::STEP_HAPPENED);
    participantList.append(PacketTypeEnum::UNDO_STEP_INITIATED);
    participantList.append(PacketTypeEnum::UNDO_STEP_APPROVED);
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

QString PacketTypeKeeperService::shouldServerHandle(QString packet)
{
    for(int i = 0; i < serverList.size(); i++){
        QString enumInStr = enumToString(serverList.at(i));
        if(packet.startsWith(enumInStr)) return enumInStr;
    }
    return "";
}

QString PacketTypeKeeperService::shouldParticipantHandle(QString packet)
{
    for(int i = 0; i < participantList.size(); i++){
        QString enumInStr = enumToString(participantList.at(i));
        if(packet.startsWith(enumInStr)) return enumInStr;
    }
    return "";
}
