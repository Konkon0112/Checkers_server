#ifndef HUMANVSROBOTROOM_H
#define HUMANVSROBOTROOM_H

#include <QObject>

#include "roombase.h"

//This one will be deleted after the game is finished
//The robot will be created in the constructor
class HumanVsRobotRoom : public RoomBase
{
    Q_OBJECT
public:
    explicit HumanVsRobotRoom(Participant::ParticipantSideEnum robotS, QObject *parent = nullptr);

    void setUpContinuedGame(Participant::ParticipantSideEnum playerOnTurn, QString stepStr);

protected slots:
    void playerQuitSlot() override;

private:
signals:
};

#endif // HUMANVSROBOTROOM_H
