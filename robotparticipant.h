#ifndef ROBOTPARTICIPANT_H
#define ROBOTPARTICIPANT_H

#include <QObject>
#include <QTcpSocket>

#include "participant.h"

class RobotParticipant : public Participant
{
    Q_OBJECT
public:
    explicit RobotParticipant(QTcpSocket* socket, ParticipantSideEnum pS , QObject *parent = nullptr);

signals:
};

#endif // ROBOTPARTICIPANT_H
