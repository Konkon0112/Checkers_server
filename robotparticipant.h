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

public slots:
    void stepHappenedSlot(QString step) override;
    void turnChangedSignal(Participant::ParticipantSideEnum nextOnTurn) override;
    void undoApprovedSlot() override;

protected slots:
    void gameStartedSlot() override;
    void turnChangedSlot() override;
};

#endif // ROBOTPARTICIPANT_H
