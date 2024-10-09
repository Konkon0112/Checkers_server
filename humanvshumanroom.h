#ifndef HUMANVSHUMANROOM_H
#define HUMANVSHUMANROOM_H

#include <QObject>
#include <QRandomGenerator>

#include "roombase.h"

//This one will reset itself when a game ends
class HumanVsHumanRoom : public RoomBase
{
    Q_OBJECT
public:
    explicit HumanVsHumanRoom(QObject *parent = nullptr);

protected slots:
    void playerQuitSlot() override;

private:
    QRandomGenerator rg = QRandomGenerator::securelySeeded();
signals:
};

#endif // HUMANVSHUMANROOM_H
