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
    void handleIncommingPacket(QString packetStr, QString packetType);

    QTcpSocket* socket;

signals:
    void undoInitiatedSignal();

public slots:
    void stepHappenedSlot(QString step) override;
    void undoApprovedSlot() override;
    void gameStartedSlot(Participant::ParticipantSideEnum nextOnTurn, QString stepsSoFar) override;
    void gameOverSlot(Participant::ParticipantSideEnum winner);
    void turnChangedSignal(Participant::ParticipantSideEnum nextOnTurn) override;

protected slots:
    void handlingReadyReadSlot();
    void turnChangedSlot() override;
};

#endif // HUMANPARTICIPANT_H
