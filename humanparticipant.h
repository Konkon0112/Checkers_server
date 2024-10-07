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
    void undoInitiatedSignal();

public slots:
    void stepHappenedSlot(QString step) override;
    void undoApprovedSlot() override;
    void gameStartedSlot() override;
    void gameOverSlot(Participant::ParticipantSideEnum winner);
    void turnChangedSignal(Participant::ParticipantSideEnum nextOnTurn) override;

protected slots:
    void handlingReadyReadSlot();
    void turnChangedSlot() override;
};

#endif // HUMANPARTICIPANT_H
