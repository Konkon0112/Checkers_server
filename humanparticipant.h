#ifndef HUMANPARTICIPANT_H
#define HUMANPARTICIPANT_H

#include <QObject>
#include <QTcpSocket>

#include "participant.h"
#include "packettypekeeperservice.h"

class HumanParticipant : public Participant
{
    Q_OBJECT
public:
    explicit HumanParticipant(QTcpSocket* socket,
                              ParticipantTypeEnum pT,
                              ParticipantSideEnum pS,
                              QObject *parent = nullptr);

private:
    QTcpSocket* socket;

signals:

protected slots:
    void handlingReadyReadSlot();
    void gameStartedSlot() override;
    void stepHappenedSlot() override;
    void gameOverSlot() override;
    void turnChangedSlot() override;
};

#endif // HUMANPARTICIPANT_H
