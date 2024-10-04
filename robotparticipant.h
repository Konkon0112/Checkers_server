#ifndef ROBOTPARTICIPANT_H
#define ROBOTPARTICIPANT_H

#include <QObject>
#include <QTcpSocket>

#include "participant.h"

class RobotParticipant : public Participant
{
    Q_OBJECT
public:
    explicit RobotParticipant(ParticipantSideEnum pS , QObject *parent = nullptr);

signals:

protected slots:
    void gameStartedSlot() override;
    void stepHappenedSlot() override;
    void gameOverSlot() override;
    void turnChangedSlot() override;
};

#endif // ROBOTPARTICIPANT_H
