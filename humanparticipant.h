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

    ~HumanParticipant();

    QTcpSocket *getSocket() const;
    bool usingThisSocket(QTcpSocket* soc) override;

private:
    void sendMessage(QByteArray message);
    void handleIncommingPacket(QString packetStr);

    QTcpSocket* socket;

signals:
    void undoInitiatedSignal();
    void rejectUndoSignal();

public slots:
    void stepHappenedSlot(QString step, Participant::ParticipantSideEnum newTurnColor) override;
    void undoNeedsApprovalSlot(Participant::ParticipantSideEnum approvingSideColor) override;
    void undoHappenedSlot(QString newStepsSoFar, Participant::ParticipantSideEnum nextC) override;
    void gameStartedSlot(Participant::ParticipantSideEnum nextOnTurn, QString stepsSoFar) override;
    void gameOverSlot(Participant::ParticipantSideEnum winner);

protected slots:
    void handlingReadyReadSlot();
};

#endif // HUMANPARTICIPANT_H
