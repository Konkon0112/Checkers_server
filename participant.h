#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <QObject>
#include <QTcpSocket>
#include "ToastTypeEnum.h"

class PacketTypeKeeperService;

class Participant : public QObject
{
    Q_OBJECT
public:
    enum class ParticipantTypeEnum {
        PLAYER,
        SPECTATOR,
    };
    enum class ParticipantSideEnum {
        DARK,
        LIGHT,
        NONE, // For spectators
    };
    explicit Participant(ParticipantTypeEnum pT,
                         ParticipantSideEnum pS,
                         QObject *parent = nullptr);

    bool isPlayerType(ParticipantTypeEnum pT);
    bool isPlayerSide(ParticipantSideEnum pC);

    ParticipantTypeEnum getPType() const;

    void setPSide(ParticipantSideEnum newPSide);

    ParticipantSideEnum getPSide() const;

    virtual bool usingThisSocket(QTcpSocket* soc) = 0;
    virtual void receiveNotification(ToastTypeEnum tt, QString msg) = 0;

protected:
    PacketTypeKeeperService* ptKeeper;
    ParticipantTypeEnum pType;
    ParticipantSideEnum pSide;

private:

signals:
    void playerQuitSignal();
    void stepInitiatedSignal(QString step);
    void approveUndoSignal();

public slots:

    virtual void gameStartedSlot(Participant::ParticipantSideEnum nextOnTurn, QString stepsSoFar) = 0;

    virtual void stepHappenedSlot(QString step, Participant::ParticipantSideEnum newTurnColor) = 0;

    virtual void undoNeedsApprovalSlot(Participant::ParticipantSideEnum approvingSideColor) = 0;

    virtual void undoHappenedSlot(QString newStepsSoFar, Participant::ParticipantSideEnum nextC) = 0;

protected slots:
};

#endif // PARTICIPANT_H
