#ifndef HUMANVSROBOTROOM_H
#define HUMANVSROBOTROOM_H

#include <QObject>

#include "roombase.h"

class HumanVsRobotRoom : public RoomBase
{
    Q_OBJECT
public:
    explicit HumanVsRobotRoom(QObject *parent = nullptr);

signals:
};

#endif // HUMANVSROBOTROOM_H
