#ifndef HUMANVSHUMANROOM_H
#define HUMANVSHUMANROOM_H

#include <QObject>

#include "roombase.h"

class HumanVsHumanRoom : public RoomBase
{
    Q_OBJECT
public:
    explicit HumanVsHumanRoom(QObject *parent = nullptr);

signals:
};

#endif // HUMANVSHUMANROOM_H
