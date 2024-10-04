#include "humanvshumanroom.h"

HumanVsHumanRoom::HumanVsHumanRoom(QObject *parent)
    : RoomBase{RoomBase::RoomState::WAITING_FOR_PLAYERS, parent}
{
    roomType = RoomBase::RoomType::HUMAN_VS_HUMAN;
}
