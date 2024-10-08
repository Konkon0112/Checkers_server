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
    bool usingThisSocket(QTcpSocket* soc) override;

signals:

public slots:
    void stepHappenedSlot(QString step) override;
    void turnChangedSignal(Participant::ParticipantSideEnum nextOnTurn) override;
    void undoApprovedSlot() override;

protected slots:
    void gameStartedSlot(Participant::ParticipantSideEnum nextOnTurn, QString stepsSoFar) override;
    void turnChangedSlot() override;
};

#endif // ROBOTPARTICIPANT_H
