#ifndef HUMANPARTICIPANT_H
#define HUMANPARTICIPANT_H

#include <QObject>
#include <QTcpSocket>

#include "participant.h"

class HumanParticipant : public Participant
{
    Q_OBJECT
public:
    explicit HumanParticipant(QTcpSocket* socket,
                              ParticipantTypeEnum pT,
                              ParticipantSideEnum pS,
                              QObject *parent = nullptr);

signals:
};

#endif // HUMANPARTICIPANT_H
